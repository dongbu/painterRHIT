#include "CytonRunner.h"

#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define POINT_EE_SET 0
#define M_PI 3.141592653
#define TIMEOUT 5000  //5 seconds

using namespace Ec;
using namespace cv;

//constructor for cyton robot object
CytonRunner::CytonRunner(int *width, int *height){
	currentX = 0;  //initialization
	currentY = 0;  //initialization
	raiseHeight = 0.08;  //roughly the length of a paintbrush?
	connected = false;  //initialization
	strokeInProgress = false; //initialization
	lastPaintColor = 0; //initialization

	//simulation size
	this->width = width; //initialization
	this->height = height; //initialization

	//current brush initialization
	curBrush = new Brush(40, 20, "ellipse");
	curBrush->setColor(200, 200, 200);
}

//deconstructor
CytonRunner::~CytonRunner(){}

//connects to the robot itself and returns success
bool CytonRunner::connect(){
	if (init()){
		printf("established connection to Cyton\n");
		connected = true;
		return true;
	}
	else{
		printf("failed to connect to Cyton\n");
		connected = false;
		return false;
	}
}

//loads a workspace from an xml.
bool CytonRunner::loadWorkspace(std::string fileLocation){
	//clear all variables.
	startJointPosition.clear();
	canvasCorners.clear();
	paint.clear();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((fileLocation).c_str());

	if (doc.child("workspace").empty()){
		printf("rejecting workspace\n");
		return false; //check to make sure the file is proper.
	}

	pugi::xml_node start = doc.child("workspace").child("starting");
	pugi::xml_node canvas = doc.child("workspace").child("canvas");
	pugi::xml_node paintPickup = doc.child("workspace").child("paintPickup");

	for (pugi::xml_node temp = start.first_child(); temp; temp = temp.next_sibling()){
		double rot = temp.attribute("s").as_double();
		startJointPosition.push_back(rot);
	}

	for (pugi::xml_node temp = canvas.first_child(); temp; temp = temp.next_sibling()){
		double x = temp.attribute("x").as_double();
		double y = temp.attribute("y").as_double();
		double z = temp.attribute("z").as_double();
		cv::Point3d corner(x, y, z);
		canvasCorners.push_back(corner);
	}
	for (pugi::xml_node temp = paintPickup.first_child(); temp; temp = temp.next_sibling()){
		double x = temp.attribute("x").as_double();
		double y = temp.attribute("y").as_double();
		int id = temp.attribute("id").as_int();
		int r = temp.attribute("r").as_int();
		int g = temp.attribute("g").as_int();
		int b = temp.attribute("b").as_int();

		cv::Point2d pos(x, y);
		std::vector<int> rgb;
		rgb.push_back(r); rgb.push_back(g); rgb.push_back(b);
		std::pair<cv::Point2d, std::vector<int>> pointAndRGB(pos, rgb);
		std::pair<int, std::pair<cv::Point2d, std::vector<int>>> paintPickup(id, pointAndRGB);
		paint.push_back(paintPickup);
	}

	regulateWorkspaceData();
	return true;
}

//puts the robot into it's starting position.
void CytonRunner::startup(){
	goToJointHome(1);
}

//puts the robot into a safe position and closes everything.
bool CytonRunner::shutdown(){
	QMessageBox message;
	message.setInformativeText("Please clear away any and all paints before continuing");
	QPushButton *okButton = message.addButton(QMessageBox::Ok);
	message.addButton(QMessageBox::Cancel);
	message.exec();
	if (message.clickedButton() == okButton){
		if (goToJointHome(0)){
			startJointPosition.clear();
			canvasCorners.clear();
			paint.clear();
			currentX = currentY = 0;
			lastPaintColor = 0;
			brushType = "";
			Ec::shutdown();
			this->connected = false;
			printf("shut down\n");
			return true;
		}
	}
	return false;
}
// sends the robot to the specified coordinates
void CytonRunner::goToPos(double x, double y, double z, bool toggle){
	EcCoordinateSystemTransformation pose;
	xScale = cWidth / *width;
	yScale = cHeight / *height;

	if (!toggle){
		std::vector<double> vec = convert(x*xScale, y*yScale, z);
		pose.setTranslationX(vec.at(0));
		pose.setTranslationY(vec.at(1));
		pose.setTranslationZ(vec.at(2));
	}
	else{
		std::vector<double> vec = convert(x*xScale, y*yScale, z);
		pose.setTranslationX(x);
		pose.setTranslationY(y);
		pose.setTranslationZ(vec.at(2));
	}
	//printf("...................................................................................................................\n");

	EcOrientation orientation;

	//roll about x-axis, pitch about y-axis,Yaw about z-axis
	orientation.setFrom123Euler(0, 0, 0);

	pose.setOrientation(orientation);

	setEndEffectorSet(FRAME_EE_SET); // point end effector set index
	//setEndEffectorSet(0);
	EcEndEffectorPlacement desiredPlacement(pose);
	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation offset, zero, actualCoord;

	//until further notice, canvas is on ground.
	zero.setTranslation(EcVector(0, 0, 0));

	//set the desired position
	setDesiredPlacement(desiredPlacement, 0, 0);

	// if it hasnt been achieved after 5 sec, return false
	EcU32 interval = 10;
	EcU32 count = 0;
	EcBoolean achieved = EcFalse;
	while (!achieved && !(count >= TIMEOUT / interval))
	{
		EcSLEEPMS(interval);
		count++;
		EcBoolean successData = getActualPlacement(actualEEPlacement);
		if (!successData) {
		}
		else {
			actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();

			//get the transformation between the actual and desired 
			offset = (actualCoord.inverse()) * pose;

			if (offset.approxEq(zero, .00001)) {
				achieved = EcTrue;
			}
			currentX = pose.translation().x();
			currentY = pose.translation().y();
		}
	}


}
//have the robot rise straight up.
void CytonRunner::raiseBrush(){
	if (!isUp){
		goToPos(currentX, currentY, raiseHeight, true);
	}
	isUp = true;
}
//have the robot lower itself to the canvas/paper/medium.
void CytonRunner::lowerBrush(){
	if (isUp){
		goToPos(currentX + 0.005, currentY + 0.005, 0.01, true);
		//almost there, let's not push the tip straight down.
		goToPos(currentX - 0.005, currentY - 0.005, 0, true);
	}
	isUp = false;
}
//have the robot retrieve paint from the paint locations.
void CytonRunner::getPaint(int paint_can_id){
	raiseBrush();
	double x, y;
	for (size_t i = 0; i < this->paint.size(); i++){
		if (this->paint.at(i).first == paint_can_id){
			x = this->paint.at(i).second.first.x;
			y = this->paint.at(i).second.first.y;
			break;
		}
	}
	goToPos(x, y, raiseHeight, true);
	lowerBrush();
	raiseBrush();

}
//have the robot draw a single point.
void CytonRunner::drawPoint(cv::Point pt){
	raiseBrush();
	goToPos(pt.x, pt.y, raiseHeight);
	lowerBrush();
	raiseBrush();
}

//have the robot draw a line between two points (a stroke one might say)
void CytonRunner::stroke(cv::Point pt1, cv::Point pt2){
	if (!strokeInProgress) {
		raiseBrush();
		goToPos(pt1.x, pt1.y, raiseHeight);
		lowerBrush();
		goToPos(pt2.x, pt2.y, 0);
		strokeInProgress = true;
	}
	else {
		goToPos(pt1.x, pt1.y, 0);
		goToPos(pt2.x, pt2.y, 0);
	}
}

//have the robot draw a multitude of strokes.
void CytonRunner::stroke(std::vector<cv::Point> pts){
	raiseBrush();
	goToPos(pts.at(0).x, pts.at(0).y, raiseHeight);
	lowerBrush();

	for (size_t i = 0; i < pts.size(); i++){
		goToPos(pts.at(i).x, pts.at(i).y, 0);
	}
	raiseBrush();
}

bool CytonRunner::goToJointHome(int type){
	//type 0 sets all joints to 0;
	EcRealVector jointPosition(7);
	if (type == 1){
		jointPosition = startJointPosition;
		isUp = true;
	}
	const EcReal angletolerance = .000001;

	EcBoolean retVal = EcTrue;
	setEndEffectorSet(JOINT_CONTROL_EE_SET);
	//setEndEffectorSet(FRAME_EE_SET);
	EcSLEEPMS(500);

	//vector of EcReals that holds the set of joint angles
	EcRealVector currentJoints;
	retVal &= getJointValues(currentJoints);

	size_t size = currentJoints.size();
	if (size < jointPosition.size()) {
		size = currentJoints.size();
	}
	else if (size >= jointPosition.size()) {
		size = jointPosition.size();
	}

	for (size_t ii = 0; ii < size; ++ii) {
		currentJoints[ii] = jointPosition[ii];
	}
	//currentJoints[size - 1] = 0.003;

	retVal &= setJointValues(currentJoints);

	//Check if achieved
	EcBooleanVector jointAchieved;
	jointAchieved.resize(size);
	EcBoolean positionAchieved = EcFalse;

	// if it hasnt been achieved after 5 sec, return false

	EcU32 interval = 10;
	EcU32 count = 0;

	while (!positionAchieved && !(count >= TIMEOUT / interval)) {
		EcSLEEPMS(interval);
		count++;
		getJointValues(currentJoints);
		for (size_t ii = 0; ii < size; ++ii) {
			if (std::abs(jointPosition[ii] - currentJoints[ii]) < angletolerance) {
				jointAchieved[ii] = EcTrue;
			}
		}
		for (size_t ii = 0; ii < size; ++ii) {
			if (!jointAchieved[ii]) {
				positionAchieved = EcFalse;
				break;
			}
			else {
				positionAchieved = EcTrue;
			}
		}
	}
	return positionAchieved;
}

//returns vector containing x, y, and z appropriate for the canvas's location.
//after recieving x, y, and z relative to top left corner of canvas.
//CHECK LATER
std::vector<double> CytonRunner::convert(double x, double y, double z){
	std::vector<double> toReturn;
	double minX, minY, minZ;
	minX = std::min(std::min(canvasCorners.at(0).x, canvasCorners.at(1).x), canvasCorners.at(2).x);
	minY = std::min(std::min(canvasCorners.at(0).y, canvasCorners.at(1).y), canvasCorners.at(2).y);
	minZ = std::min(std::min(canvasCorners.at(0).z, canvasCorners.at(1).z), canvasCorners.at(2).z);

	toReturn.push_back(minX + x);
	toReturn.push_back(minY + y);
	toReturn.push_back(minZ + z);

	return toReturn;

	//need to figure out plane transformations
	//double xNew = x*cos(theta) + z*sin(theta) + y*sin(psi) + x*sin(psi) + x1 + dx;
	//double yNew = y*cos(phi) + z*sin(phi) + y*sin(psi) - x*sin(psi) + y1 + dy;
	//double zNew = -y*sin(phi) + (z / 2.0)*(cos(phi) + cos(theta)) - x*sin(theta) + z1 + dz;

}

//set size of simulation.
void CytonRunner::setSimulationSize(int *width, int *height){
	this->width = width;
	this->height = height;
}

//set size of actual canvas.
void CytonRunner::setCanvasSize(double width, double height){
	this->cWidth = width;
	this->cHeight = height;
	xScale = cWidth / *this->width;
	yScale = cHeight / *this->height;
}

//used to startup the workspace wizard.
void CytonRunner::createWorkspace(){
	this->goToJointHome(0);
	printf("Previous \"Workspace Wizard\" has been removed as it was bad\n");
	printf("the only way to create the workspace at current is via text editing\n");
}

//fixes the robot to paint things in the canvas region.
void CytonRunner::regulateWorkspaceData() {
	double minX, maxX, minY, maxY;

	minX = std::min(std::min(canvasCorners.at(0).x, canvasCorners.at(1).x), canvasCorners.at(2).x);
	minY = std::min(std::min(canvasCorners.at(0).y, canvasCorners.at(1).y), canvasCorners.at(2).y);
	maxX = std::max(std::max(canvasCorners.at(0).x, canvasCorners.at(1).x), canvasCorners.at(2).x);
	maxY = std::max(std::max(canvasCorners.at(0).y, canvasCorners.at(1).y), canvasCorners.at(2).y);
	this->setCanvasSize(maxX - minX, maxY - minY);
}

//let things know it is finished with a line/drawing/etc.
void CytonRunner::tellFinished() {
	emit finishedSettingWorkspace();
}

//change to a new paint color.
void CytonRunner::changePaint(int new_paint_can_id){
	if (new_paint_can_id == lastPaintColor || lastPaintColor == 0){
		//just used this color, don't need to wash.
		Sleep(100);//pause 1/10th of a second to let things settle
		getPaint(new_paint_can_id);
		lastPaintColor = new_paint_can_id;
		return;
	}
	lastPaintColor = new_paint_can_id;
	if (new_paint_can_id > (int)this->paint.size()){
		new_paint_can_id = (int)this->paint.size();
		printf("paint can ID out of bounds. Substituting color\n");
	}
	if (paint.size() > 0){
		getPaint(paint.at(0).first); //paint at 0 is water
	}
	lowerBrush();
	double x = paint.at(0).second.first.x;
	double y = paint.at(0).second.first.y;
	double z = 0.0095;

	for (int i = -1; i < 2; i++){
		for (double k = 0.0; k <= 2.0 * M_PI; k += 0.2){
			goToPos(x + 0.01*i*sin(k), y + 0.01*i*cos(k), z, true);
			if (i == 0 && k <= 0.8){
				goToPos(x + 0.01*cos(k), y + 0.01*sin(k), z, true);
			}
		}
	}
	raiseBrush();
	Sleep(10000); //pause for 10 seconds to give a chance for water to drip down. (and for me to dry it of if I want).
	getPaint(new_paint_can_id);
}

//Takes target rgb values, and has the robot select the color with the closest rgb.
void CytonRunner::decidePaint(int r, int g, int b){
	int closestId = 0;
	int closestNum = -1;
	//loop to find smallest distance
	for (int i = 1; i < paint.size(); i++){
		int r1, g1, b1;
		r1 = paint.at(i).second.second.at(0);
		g1 = paint.at(i).second.second.at(1);
		b1 = paint.at(i).second.second.at(2);
		int distance = sqrt(pow(r - r1, 2) + pow(g - g1, 2) + pow(b - b1, 2));
		if (distance < closestNum || closestNum == -1){
			closestId = i;
			closestNum = distance;

		}
	}
	int r1 = paint.at(closestId).second.second.at(0);
	int g1 = paint.at(closestId).second.second.at(1);
	int b1 = paint.at(closestId).second.second.at(2);
	//set color then physically get the color.
	this->curBrush->setColor(b1, g1, r1);
	changePaint(closestId);
}