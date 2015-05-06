#include <iostream>
#include <math.h>
#include <String>
#include <opencv2/opencv.hpp>
#include <boost/thread/tss.hpp>

#include <control/ecEndEffectorSet.h>
#include <control/ecFrameEndEffector.h>
#include <control/ecManipEndEffectorPlace.h>
#include <foundCommon/ecCoordSysXForm.h>
#include <foundCore/ecApplication.h>
#include <foundCore/ecConstants.h>
#include <foundCore/ecMacros.h>
#include <foundCore/ecTypes.h>
#include <manipulation/ecManipulationActionManager.h>
#include <manipulationDirector/ecManipulationScript.h>
#include <remoteCommand/ecRemoteCommand.h>

# define M_PI 3.14159265358979323846  /* pi */

#define POINT_EE_SET 0
#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define COLORS 4
#define SOURCE_COLOR1	Point3d(0.2586, -0.0693, 0.0682)
#define SOURCE_COLOR2	Point3d(0.1938, -0.0579, 0.0682)
#define SOURCE_COLOR3	Point3d(0.1938, -0.0579, 0.0682)
#define DROPOFF		Point3d(0.2586, -0.0693, 0.0682)
#define PICKUP		Point3d(0.1938, -0.0579, 0.0682)

using namespace cv;
using namespace std;
using namespace Ec;

void net_move(Point3d p);
void swap_brush(Point3d, Point3d);
void get_paint(Point3d paintSource);
void stroke();
bool moveGripper(const double gripperPos);
vector<Point3d> makeCircle(float z);
Point3d getLeadIn(vector<Point3d> contour);

int main(int argc, char* argv[])
{
	string inputFilePath = "C:\\Pictures\\logo.bmp";

	int cannyThresh = 240;

	if (argc > 1) {
		inputFilePath = argv[1];
	}

	const Point3d offset = Point3d(0.1, -0.2, 0.1);
	const double scale = 0.0006;
	const Point3d liftOff = Point3d(0, 0, 0.02);

	const Point3d Brush1Position;
	const Point3d Brush2Position;

	Mat greyscaleImage = imread(inputFilePath);
	assert(greyscaleImage.data != NULL);

	//imshow("Input Image", greyscaleImage);
	Mat cannyOutput;
	Canny(greyscaleImage, cannyOutput, cannyThresh, cannyThresh * 2);
	assert(cannyOutput.data != NULL);
	//imshow("Canny", cannyOutput);

	vector<vector<Point>> contours;
	findContours(cannyOutput, contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_L1);

	//vector<pair<vector<Point>, int>> newFormat1; 
	//newFormat[0].first = the path
	//newFormat[0].second = the color index

	//code here to convert to
	vector<pair<vector<Point3d>, int>> newFormat;
	int colorIndex = 0;
	for (vector<Point> i : contours) {
		vector<Point3d> tmpPath;
		newFormat.push_back(make_pair(tmpPath, colorIndex));
		colorIndex += 1;
		colorIndex %= 4;
		//cout << "new path" << endl;
		for (Point j : i) {
			Point3d k = Point3d(j.x, j.y, 0);
			k = k*scale;
			k = k + offset;
			tmpPath.push_back(k);
			//cout << k << endl;
		}
	}
	
	if (init()) {
		cout << "Established Connection" << endl;
	}else {
		cout << "Error establishing connection" << endl;
		exit(1);
	}
	
	Point3d leadin, lastPoint, source;
	vector<Point3d> circle = makeCircle(newFormat[0].first[0].z);
	cout << "Moving through points" << endl;
	for (int i = 1; i < COLORS; i++) {
		for (unsigned int j = 0; j < newFormat.size(); j++) {
			if (newFormat[j].second == i) {
				// Get more paint				
				if (i == 1){source = SOURCE_COLOR1;}
				else if (i == 2){source = SOURCE_COLOR2;}
				else if (i == 3){source = SOURCE_COLOR3;}

				net_move(source + Point3d(0, 0, 0.02));
				net_move(source);
				// Move in a circle
				for (int n = 0; n < circle.size(); n++){
					net_move(source + circle[n]);
				}
				net_move(source + Point3d(0, 0, 0.02));

				// Lead-in motion
				net_move(getLeadIn(newFormat[j].first));
				
				for (Point3d k : newFormat[j].first) {
					net_move(k);
					lastPoint = k;
				}
				net_move(lastPoint + Point3d(0, 0, 0.02));
			}
		}
		swap_brush(DROPOFF, PICKUP);
	}
	cout << "Done moving" << endl;

	//imshow("Canny Output", cannyOutput);
	//waitKey(0);

	return 0;
}

Point3d getLeadIn(vector<Point3d> contour){
	Point3d leadin;
	int dx, dy, dist, lx, ly;
	dy = (contour[1].y - contour[0].y);
	dx = (contour[1].x - contour[0].x);
	dist = sqrt(dx*dx + dy*dy);
	lx = contour[0].x - 0.02 * (dx / dist);
	ly = contour[0].y - 0.02 * (dy / dist);
	leadin = Point3d(lx, ly, (contour[0].z + 0.01));
	return leadin;
}

vector<Point3d> makeCircle(float z){
	vector<Point3d> circle;
	float cm = 0.01;
	for (float i = 0; i < 2*M_PI; i += M_PI/4){
		circle[i] = Point3d(cm*cos(i), cm*sin(i), z);
	}
	return circle;
}

void get_paint(Point3d paintSource){
	net_move(paintSource + Point3d(0, 0, 0.1));
	net_move(paintSource);
	net_move(paintSource + Point3d(0, 0, 0.1));
}

void swap_brush(Point3d brushDropoff, Point3d newBrush) {
	net_move(brushDropoff + Point3d(0, 0, 0.1));
	net_move(brushDropoff);
	moveGripper(0.002);
	net_move(brushDropoff + Point3d(0, 0, 0.1));
	net_move(newBrush + Point3d(0, 0, 0.1));
	// imshow
	waitKey(0);
	net_move(newBrush);
	moveGripper(0.000);
	net_move(newBrush + Point3d(0, 0, 0.1));
}

void net_move(Point3d p) {
	EcCoordinateSystemTransformation pose;
	pose.setTranslationX(p.x);
	pose.setTranslationY(p.y);
	pose.setTranslationZ(p.z);
	EcOrientation orientation;
	orientation.setFrom123Euler(0, 0, 0);
	pose.setOrientation(orientation);
	std::cout << "Desired pose:  x: " << pose.translation().x() << " y: " << pose.translation().y() << " z: " << pose.translation().z() << "...";

	setEndEffectorSet(FRAME_EE_SET); // point end effector set index
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
}

bool moveGripper(const double gripperPos) {
	EcManipulatorEndEffectorPlacement actualEEPlacement, desiredEEPlacement;
	setEndEffectorSet(FRAME_EE_SET);
	EcSLEEPMS(100);
	getActualPlacement(actualEEPlacement);

	//0 is the Wrist roll link (point or frame end effector), 
	//1 is the first gripper finger link (linear constraint end effector)
	EcEndEffectorPlacementVector state = actualEEPlacement.offsetTransformations();

	//set the translation of the driving gripper finger
	EcCoordinateSystemTransformation gripperfinger1trans = state[1].coordSysXForm();
	gripperfinger1trans.setTranslation(EcVector(0, 0, gripperPos));
	EcEndEffectorPlacement finger1placement = state[1];
	finger1placement.setCoordSysXForm(gripperfinger1trans);
	state[1] = finger1placement;

	desiredEEPlacement.setOffsetTransformations(state);

	//set the desired placement
	setDesiredPlacement(desiredEEPlacement, 0);

	// if it hasnt been achieved after 2 sec, return false
	EcU32 timeout = 2000;
	EcU32 interval = 10;
	EcU32 count = 0;
	bool achieved = EcFalse;
	while (!achieved && !(count >= timeout / interval))
	{
		EcSLEEPMS(interval);
		count++;

		//EcPrint(Debug) << "Moving " << std::endl;

		getActualPlacement(actualEEPlacement);
		EcEndEffectorPlacementVector currentState = actualEEPlacement.offsetTransformations();
		EcCoordinateSystemTransformation gripperfinger1trans = currentState[1].coordSysXForm();
		EcReal difference = abs(gripperPos - gripperfinger1trans.translation().z());
		//EcPrint(Debug) << "distance between actual and desired: " << difference << std::endl;

		if (difference < .000001)
		{
			achieved = EcTrue;
		}
	}
	//std::cout << (achieved ? "Achieved Gripper Position" : "Failed to Achieve Gripper Position") << std::endl;
	return achieved;
}


