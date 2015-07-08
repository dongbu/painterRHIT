#include "CytonController.h"
#include <iostream>
#include <qmessagebox>

#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define POINT_EE_SET 0

using namespace Ec;
using namespace cv;
/**
 * @brief constructor for cyton.
 * initializes points for "home"
 */
CytonController::CytonController()
{
	s1 = -2;
	s2 = -1;
	s3 = 0;
	s4 = -0.5;
	s5 = -1.1;
	s6 = -1.2;
	s7 = 0.02;

}

/**
 * @brief virtual destructor.
 */
CytonController::~CytonController()
{
	go_home(0);
}

/**
 * @brief connects to the robot.
 * @return bool
 */
bool CytonController::connect(){
	if (init()){
		std::cout << "established connection" << std::endl;

		go_home(0);
		return true;
	}
	else{
		std::cout << "Failed to establish connection" << std::endl;
		QMessageBox box;
		box.setText("failed to establish connection to robot");
		box.setInformativeText("have a nice day");
		box.exec();
		return false;
	}
}

/**
 * @brief moves the robot to a given point using a specific endEffectorSet
 * @param p
 * @param endEffectorSet
 * @return bool
 */
bool CytonController::net_move(Point3d p, int endEffectorSet) {
	EcCoordinateSystemTransformation pose;
	pose.setTranslationX(p.x);
	pose.setTranslationY(p.y);
	pose.setTranslationZ(p.z);
	EcOrientation orientation;
	if (endEffectorSet == FRAME_EE_SET){
	orientation.setFrom123Euler(0, 0, 0);
	pose.setOrientation(orientation);
	}
	std::cout << "Desired pose:  x: " << pose.translation().x() << " y: " << pose.translation().y() << " z: " << pose.translation().z() << "...";

	setEndEffectorSet(endEffectorSet); // point end effector set index
	EcEndEffectorPlacement desiredPlacement(pose);
	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation offset, zero, actualCoord;
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

		if (offset.approxEq(zero, .00001))
		{
			achieved = EcTrue;
		}

	}
	std::cout << (achieved ? "Achieved Pose" : "Failed to Achieve Pose") << std::endl;

	return achieved;
}

/**
* @brief if pos is 0, puts all joints at 0.  if pos is 1, sets the robot in a position to draw.
* 
*/
void CytonController::go_home(int pos){
	EcRealVector jointPosition(7);
	if (pos == 1){
		jointPosition[0] = s1;
		jointPosition[1] = s2;
		jointPosition[2] = s3;
		jointPosition[3] = s4;
		jointPosition[4] = s5;
		jointPosition[5] = s6;
		jointPosition[6] = s7;

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

		EcPrint(Debug) << "Moving ";
		getJointValues(currentJoints);
		EcPrint(Debug) << "Current Joints: ";
		for (size_t ii = 0; ii<size; ++ii)
		{

			EcPrint(Debug) << " , " << currentJoints[ii];

			if (std::abs(jointPosition[ii] - currentJoints[ii])<angletolerance)
			{
				jointAchieved[ii] = EcTrue;
			}
		}
		EcPrint(Debug) << std::endl;
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


	std::cout << (positionAchieved ? "Achieved Joint State" : "Failed to Achieve Joint State") << std::endl;

}

/**
* @brief converts pixels to actual coordinates.
*/
Point3d CytonController::convert(double x, double y, double z){
	const Point3d offset = Point3d(0.0468, -0.3137, 0.0625);
	double scale = 0.0001;
	double xCoord = x* scale;
	double yCoord = y*scale;
	return (Point3d(xCoord, yCoord, z) + offset);
	
}

/**
* @brief tells the robot to trace the path based on pixels.
*/
bool CytonController::traceLine(double x1, double y1, double x2, double y2, bool continuous, bool prevContinuous, int current){

	double drawHeight = 0;
	double moveHeight = 0.1;

	Point3d firstPointUp = convert(x1, y1, moveHeight);
	Point3d firstPointDown = convert(x1, y1, drawHeight);
    Point3d secondPointDown = convert(x2, y2, drawHeight);
	Point3d secondPointUp = secondPointDown;
	
	if (!continuous){
		secondPointUp = convert(x2, y2, moveHeight);
	}
	if (prevContinuous){
		firstPointUp = firstPointDown;
	}

	bool achieved = true;
	if (!net_move(firstPointUp, FRAME_EE_SET)){
		achieved = false;
	}
	if (net_move(firstPointDown, FRAME_EE_SET)){
		achieved = false;
	}
	if (!net_move(secondPointDown, FRAME_EE_SET)){
		achieved = false;
	}
	if (!net_move(secondPointUp, FRAME_EE_SET)){
		achieved = false;
	}
	emit finishedLine(current);
	return achieved;
}
