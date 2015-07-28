#include "CytonRunner.h"
#include <qmessagebox>
#include <qpushbutton.h>
//#include <qobject.h>
#include "workspacewizard.h";

#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define POINT_EE_SET 0
#define M_PI 3.141592653

using namespace Ec;
using namespace cv;


CytonRunner::CytonRunner(int width, int height, QWidget *parent)
//:QDialog(parent),
//ui(new Ui::CytonRunner)
{
	//ui = Ui::CytonRunner;
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

CytonRunner::~CytonRunner()
{
//	delete ui;
}

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


void CytonRunner::loadWorkspace(std::string fileLocation){
	//clear all variables.
	startJointPosition.clear();
	canvasCorners.clear();
	paint.clear();
	dx = dy = dz = 0;
	brushType = "";

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((fileLocation).c_str());

	pugi::xml_node start = doc.child("workspace").child("starting");
	pugi::xml_node canvas = doc.child("workspace").child("canvas");
	pugi::xml_node paintPickup = doc.child("workspace").child("paintPickup");
	pugi::xml_node brush = doc.child("workspace").child("brush");

	for (pugi::xml_node temp = start.first_child(); temp; temp = temp.next_sibling()){
		double rot = temp.attribute("s").as_double();
		startJointPosition.push_back(rot);
	}

	for (pugi::xml_node temp = canvas.first_child(); temp; temp = temp.next_sibling()){
		double x = temp.attribute("x").as_double();
		double y = temp.attribute("y").as_double();
		double z = temp.attribute("z").as_double();
		cv::Point3d corner(x,y,z);
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

	dx = brush.attribute("dx").as_double();
	dy = brush.attribute("dy").as_double();
	dz = brush.attribute("dz").as_double();

	brushType = brush.next_sibling().attribute("type").as_string();

	//figure out roll, pitch, and yaw.  Not used as of yet.
	this->phi = 0;
	this->theta = 0;
	this->psi = 0;


}

void CytonRunner::startup(){
	goToJointHome(1);
}
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
			dx = dy = dz = 0;
			currentX = currentY = 0;
			brushType = "";
			Ec::shutdown();
			printf("shut down\n");
			return true;
		}
	}
	return false;
}
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
void CytonRunner::raiseBrush(){
	if (!isUp){
		goToPos(currentX, currentY, raiseHeight);
	}
	isUp = true;
}
void CytonRunner::lowerBrush(){
	if (!isUp) return;

	goToPos(currentX + 0.01, currentY + 0.01, 0.01);
	//almost there, let's not push the tip straight down.
	goToPos(currentX - 0.01, currentY - 0.01, 0);
	isUp = false;
}
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
void CytonRunner::drawPoint(cv::Point pt){
	raiseBrush();
	goToPos(pt.x, pt.y, raiseHeight);
	lowerBrush();
	raiseBrush();
}

void CytonRunner::stroke(cv::Point pt1, cv::Point pt2){
	raiseBrush();
	goToPos(pt1.x, pt1.y, raiseHeight);
	lowerBrush();
	goToPos(pt2.x, pt2.y, 0);
}

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
	EcSLEEPMS(500);

	//vector of EcReals that holds the set of joint angles
	EcRealVector currentJoints;
	retVal &= getJointValues(currentJoints);

	size_t size = currentJoints.size();
	if (size < jointPosition.size()) {
		size = currentJoints.size();
	} else if (size >= jointPosition.size()) {
		size = jointPosition.size();
	}

	for (size_t ii = 0; ii<size; ++ii) {
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
		for (size_t ii = 0; ii<size; ++ii) {
			if (std::abs(jointPosition[ii] - currentJoints[ii])<angletolerance) {
				jointAchieved[ii] = EcTrue;
			}
		}
		for (size_t ii = 0; ii<size; ++ii) {
			if (!jointAchieved[ii]) {
				positionAchieved = EcFalse;
				break;
			} else {
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
	double x1 = canvasCorners.at(0).x;
	double y1 = canvasCorners.at(0).y;
	double z1 = canvasCorners.at(0).z;


	//need to figure out plane transformations
	//double xNew = x*cos(theta) + z*sin(theta) + y*sin(psi) + x*sin(psi) + x1 + dx;
	//double yNew = y*cos(phi) + z*sin(phi) + y*sin(psi) - x*sin(psi) + y1 + dy;
	//double zNew = -y*sin(phi) + (z / 2.0)*(cos(phi) + cos(theta)) - x*sin(theta) + z1 + dz;

	double xNew = x + x1 + dx;
	double yNew = y + y1 + dy;
	double zNew = z + z1 + dz;

	std::vector<double> temp;
	temp.push_back(xNew);
	temp.push_back(yNew);
	temp.push_back(zNew);

	return temp;
}

void CytonRunner::setSimulationSize(int width, int height){
	this->width = width;
	this->height = height;
}

void CytonRunner::setCanvasSize(double width, double height){
	this->cWidth = width;
	this->cHeight = height;
	xScale = cWidth / width;
	yScale = cHeight / height;
}

void CytonRunner::paintShape(Shape *s){
	int border = 30;
	printf("about to draw %s\n", s->type.c_str());
	printf("press enter to continue\n");
	std::cin.ignore();
	if (s->fill){
		printf("filled\n");
		PixelRegion *p = s->toPixelRegion();
		RegionToPaths RTP = RegionToPaths(width, height, border);
		RTP.clear();
		
		for (size_t i = 0; i < p->getPoints().size(); i++){
			cv::Point currentPoint = p->getPoints().at(i);
			RTP.addDesiredPixel(currentPoint.x, currentPoint.y);
		}
		Brush brush = Brush(this->dx * 2, this->dy * 2, this->brushType);
		brush.setColor(s->getPenColor());
		
		RTP.defineBrush(&brush);
		std::vector<std::vector<cv::Point>> pathVec = RTP.getBrushStrokes();

		for (size_t i = 0; i < pathVec.size(); i++){
			this->stroke(pathVec.at(i));
		}		
	}
	else if (!s->fill){
		printf("not filled\n");
		PolyLine *p = s->toPolyline();
		this->stroke(p->getPoints());
	}
	else{
		printf("fill is %i\n",s->fill);
	}

	emit finishedShape();
}

void CytonRunner::createWorkspace(){
	printf("hi\n");
	WorkspaceWizard *w = new WorkspaceWizard();
	w->show();
}

void CytonRunner::moveDirection(int direction){

}
