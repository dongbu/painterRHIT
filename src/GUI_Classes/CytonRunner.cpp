#include "CytonRunner.h"
#include "workspacewizard.h"

#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define POINT_EE_SET 0
#define M_PI 3.141592653

using namespace Ec;
using namespace cv;

//constructor for cyton robot object
CytonRunner::CytonRunner(int width, int height){
	currentX = 0;
	currentY = 0;
	raiseHeight = 0.1;
	connected = false;

	//simulation size
	this->width = width;
	this->height = height;

	//canvas size
	cWidth = 0.3; //meters
	cHeight = 0.3; //meters
	xScale = cWidth / width;
	yScale = cHeight / height;
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
		double id = temp.attribute("id").as_int();

		cv::Point pos(x, y);
		std::pair<int, cv::Point> paintPickup(id, pos);
		paint.push_back(paintPickup);
	}

	//figure out roll, pitch, and yaw.  Not used as of yet.
	this->phi = 0;
	this->theta = 0;
	this->psi = 0;

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
			brushType = "";
			Ec::shutdown();
			printf("shut down\n");
			return true;
		}
	}
	return false;
}
// sends the robot to the specified coordinates
void CytonRunner::goToPos(double x, double y, double z){
	EcCoordinateSystemTransformation pose;

	std::vector<double> vec = convert(x*xScale, y*yScale, z);

	pose.setTranslationX(vec.at(0));
	pose.setTranslationY(vec.at(1));
	pose.setTranslationZ(vec.at(2));
	EcOrientation orientation;

	//roll about x-axis, pitch about y-axis,Yaw about z-axis
	orientation.setFrom123Euler(0, 0, 0);

	pose.setOrientation(orientation);

	setEndEffectorSet(FRAME_EE_SET); // point end effector set index
	EcEndEffectorPlacement desiredPlacement(pose);
	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation offset, zero, actualCoord;

	//until further notice, canvas is on ground.
	zero.setTranslation(EcVector(0, 0, 0));

	//set the desired position
	setDesiredPlacement(desiredPlacement, 0, 0);

	// if it hasnt been achieved after 5 sec, return false
	EcU32 timeout = 5000;
	EcU32 interval = 10;
	EcU32 count = 0;
	EcBoolean achieved = EcFalse;
	while (!achieved && !(count >= timeout / interval))
	{
		EcSLEEPMS(interval);
		count++;

		getActualPlacement(actualEEPlacement);
		actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();

		//get the transformation between the actual and desired 
		offset = (actualCoord.inverse()) * pose;

		if (offset.approxEq(zero, .00001)) achieved = EcTrue;

		currentX = x;
		currentY = y;
	}

}
//have the robot rise straight up.
void CytonRunner::raiseBrush(){
	if (!isUp){
		goToPos(currentX, currentY, raiseHeight);
	}
	isUp = true;
}
//have the robot lower itself to the canvas/paper/medium.
void CytonRunner::lowerBrush(){
	if (!isUp) return;

	goToPos(currentX + 0.01, currentY + 0.01, 0.01);
	//almost there, let's not push the tip straight down.
	goToPos(currentX - 0.01, currentY - 0.01, 0);
	isUp = false;
}
//have the robot retrieve paint from the paint locations.
void CytonRunner::getPaint(int paint_can_id){
	raiseBrush();
	double x, y;
	for (size_t i = 0; i < this->paint.size(); i++){
		if (this->paint.at(i).first == paint_can_id){
			x = this->paint.at(i).second.x;
			y = this->paint.at(i).second.y;
			break;
		}
	}
	lowerBrush();
	raiseBrush();
	goToPos(x, y, raiseHeight);
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
	raiseBrush();
	goToPos(pt1.x, pt1.y, raiseHeight);
	lowerBrush();
	goToPos(pt2.x, pt2.y, 0);
}

//have the robot draw a multitude of strokes.
void CytonRunner::stroke(std::vector<cv::Point> pts){
	printf("stroking...\n");
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

	retVal &= setJointValues(currentJoints);

	//Check if achieved
	EcBooleanVector jointAchieved;
	jointAchieved.resize(size);
	EcBoolean positionAchieved = EcFalse;

	// if it hasnt been achieved after 5 sec, return false
	EcU32 timeout = 5000;
	EcU32 interval = 10;
	EcU32 count = 0;

	while (!positionAchieved && !(count >= timeout / interval)) {
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
	toReturn.push_back(minZ + z + 0.01);

	return toReturn;

	//need to figure out plane transformations
	//double xNew = x*cos(theta) + z*sin(theta) + y*sin(psi) + x*sin(psi) + x1 + dx;
	//double yNew = y*cos(phi) + z*sin(phi) + y*sin(psi) - x*sin(psi) + y1 + dy;
	//double zNew = -y*sin(phi) + (z / 2.0)*(cos(phi) + cos(theta)) - x*sin(theta) + z1 + dz;

}

void CytonRunner::setSimulationSize(int width, int height){
	this->width = width;
	this->height = height;
}

void CytonRunner::setCanvasSize(double width, double height){
	this->cWidth = width;
	this->cHeight = height;
	xScale = cWidth / this->width;
	yScale = cHeight / this->height;
}

void CytonRunner::paintShape(Shape *s){
	int border = 30;
	if (s->fill){ //painting a filled object
		RegionToPaths RTP = RegionToPaths(width, height, border);

		PixelRegion *p = s->toPixelRegion();
		std::vector<cv::Point> pts = p->getPoints();

		for (size_t i = 0; i < pts.size(); i++){ RTP.addDesiredPixel(pts.at(i).x, pts.at(i).y); }

		Brush brush = Brush(30, 20, "ellipse");
		brush.setColor(20, 20, 40);
		RTP.defineBrush(&brush);
		RTP.definePaths();

		std::vector<std::vector<cv::Point>> pathVec = RTP.getBrushStrokes();
		for (size_t i = 0; i < pathVec.size(); i++){ this->stroke(pathVec.at(i)); }
	}
	else { //painting a PolyLine object
		PolyLine *p = s->toPolyline();
		this->stroke(p->getPoints());
	}

	emit finishedShape();
}

//used to startup the workspace wizard.
void CytonRunner::createWorkspace(){
	this->goToJointHome(0);
	WorkspaceWizard *w = new WorkspaceWizard(this);
	w->show();
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

void CytonRunner::tellFinished() { emit finishedSettingWorkspace(); }