#include "CytonRunner.h"
#include <qmessagebox>
#include <qpushbutton.h>

#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define POINT_EE_SET 0

using namespace Ec;
using namespace cv;


CytonRunner::CytonRunner()
{
}


CytonRunner::~CytonRunner()
{
}

bool CytonRunner::connect(){
	if (init()){
		printf("established connection to Cyton\n");
		return true;
	}
	else{
		printf("failed to connect to Cyton\n");
	}
	return false;
}


void CytonRunner::loadWorkspace(std::string fileLocation){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((fileLocation).c_str());

	pugi::xml_node start = doc.child("workspace").child("starting");
	pugi::xml_node canvas = doc.child("workspace").child("canvas");
	pugi::xml_node paintPickup = doc.child("workspace").child("paintPickup");
	pugi::xml_node brush = doc.child("workspace").child("brush");

	for (pugi::xml_node temp = start; temp; temp = temp.next_sibling()){
		startJointPosition.push_back(temp.attribute("s").as_double());
	}
	for (pugi::xml_node temp = canvas; temp; temp = temp.next_sibling()){
		int x = temp.attribute("x").as_double();
		int y = temp.attribute("y").as_double();
		std::pair<int, int> corner(x,y);
		canvasCorners.push_back(corner);
	}
	for (pugi::xml_node temp = paintPickup; temp; temp = temp.next_sibling()){
		paint;
		int x = temp.attribute("x").as_double();
		int y = temp.attribute("y").as_double();
		int id = temp.attribute("id").as_int();

		std::pair<int, int> pos(x, y);
		std::pair<int, std::pair<int, int>> paintPickup(id, pos);
		paint.push_back(paintPickup);
	}
	
	dx = brush.next_sibling().attribute("dx").as_double();
	dy = brush.next_sibling().attribute("dy").as_double();
	dz = brush.next_sibling().attribute("dz").as_double();
	brushType = brush.next_sibling().attribute("type").as_string();


}
void CytonRunner::createWorkspace(){

}
void CytonRunner::saveWorkspace(){

}
void CytonRunner::startup(){
	goToJointHome(1);
}
void CytonRunner::shutdown(){
	QMessageBox message;
	message.setInformativeText("Please clear away any and all paints before continuing");
	QPushButton *okButton = message.addButton(QMessageBox::Ok);
	QPushButton *cancelButton = message.addButton(QMessageBox::Cancel);
	message.exec();
	if(message.clickedButton() == okButton){
		if (goToJointHome(0)){
			Ec::shutdown();
			printf("shut down\n");
		}
	}
}
void CytonRunner::goToPos(int x, int y){

}
void CytonRunner::raiseBrush(){

}
void CytonRunner::lowerBrush(){

}
void CytonRunner::getPaint(int paint_can_id){

}
void CytonRunner::drawPoint(std::pair<int, int> pt){

}
void CytonRunner::stroke(std::pair<int, int> pt1, std::pair<int, int> pt2){

}

bool CytonRunner::goToJointHome(int type){
	//type 0 sets all joints to 0;
	EcRealVector jointPosition(7);
	if (type == 1){
		jointPosition = startJointPosition;
	}
	const EcReal angletolerance = .000001;

	EcBoolean retVal = EcTrue;
	setEndEffectorSet(JOINT_CONTROL_EE_SET);
	EcSLEEPMS(500);

	//vector of EcReals that holds the set of joint angles
	EcRealVector currentJoints;
	retVal &= getJointValues(currentJoints);

	size_t size = currentJoints.size();
	if (size < jointPosition.size())
	{
		size = currentJoints.size();
	}
	else if (size >= jointPosition.size())
	{
		size = jointPosition.size();
	}

	for (size_t ii = 0; ii<size; ++ii)
	{
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

	while (!positionAchieved && !(count >= timeout / interval))
	{
		EcSLEEPMS(interval);
		count++;
		getJointValues(currentJoints);
		for (size_t ii = 0; ii<size; ++ii)
		{

			if (std::abs(jointPosition[ii] - currentJoints[ii])<angletolerance)
			{
				jointAchieved[ii] = EcTrue;
			}
		}

		for (size_t ii = 0; ii<size; ++ii)
		{
			if (!jointAchieved[ii])
			{
				positionAchieved = EcFalse;
				break;
			}
			else
			{
				positionAchieved = EcTrue;
			}
		}
	}
	return positionAchieved;
}