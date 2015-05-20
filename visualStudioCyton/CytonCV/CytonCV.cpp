#include <iostream>
#include <math.h>
#include <String>
#include <opencv2/opencv.hpp>
#include <boost/thread/tss.hpp>
#include "ImageToStroke.h"

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

# define M_PI 3.141592653  /* pi */

#define POINT_EE_SET 0
#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF
#define COLORS 4
#define SOURCE_COLOR0	Point3d(0.2586, -0.0693, 0.0682)
#define SOURCE_COLOR1	Point3d(0.2586, -0.0693, 0.0682)
#define SOURCE_COLOR2	Point3d(0.1938, -0.0579, 0.0682)
#define SOURCE_COLOR3	Point3d(0.1938, -0.0579, 0.0682)
#define DROPOFF		Point3d(0.2586, -0.0693, 0.0682)
#define PICKUP		Point3d(0.2586, -0.0693, 0.0682)

using namespace cv;
using namespace std;
using namespace Ec;

void net_move(Point3d p);
void swap_brush(Point3d, Point3d);
void get_paint(Point3d paintSource);
void stroke();
bool moveGripper(const double gripperPos);
vector<Point3d> makeCircle(double z);
Point3d getLeadIn(vector<Point3d> contour);

int main(int argc, char* argv[])
{
	VideoCapture stream1(0);
	stream1.set(CV_CAP_PROP_FRAME_WIDTH, 400);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera 1";
	}
	string inputFilePath = "C:\\Pictures\\logo.bmp";

	int cannyThresh = 240;

	if (argc > 1) {
		inputFilePath = argv[1];
	}

	const Point3d offset = Point3d(0.0468, -0.3137, 0.0682);
	const double scale = 0.05;
	const Point3d liftOff = Point3d(0, 0, 0.02);

	const Point3d Brush1Position;
	const Point3d Brush2Position;

	Mat image;
	//image = imread(inputFilePath);
	while (true) {
		stream1.read(image);
		imshow("cam", image);
		if (waitKey(30) >= 0)
			break;
	}
	stream1.release();

	assert(image.data != NULL);
	imshow("Image", image);
	waitKey(0);

	//imshow("Input Image", greyscaleImage);
	//Mat cannyOutput;
	/*Canny(image, cannyOutput, cannyThresh, cannyThresh * 2);
	assert(cannyOutput.data != NULL);*/
	//imshow("Canny", cannyOutput);

	//vector<vector<Point>> contours;
	//findContours(cannyOutput, contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_L1);

	//vector<pair<vector<Point>, int>> newFormat1; 
	//newFormat[0].first = the path
	//newFormat[0].second = the color index

	vector<pair<vector<Point>, int> > contours = imageToStroke(image);

	//code here to convert to
	vector<pair<vector<Point3d>, int>> newFormat;
	int colorIndex = 0;
	for (pair<vector<Point>, int > i : contours) {
		vector<Point3d> tmpPath;
		//cout << "new path" << endl;
		for (Point j : i.first) {
			Point3d k = Point3d(j.x, j.y, 0);
			k = k*scale;
			k = k + offset;
			tmpPath.push_back(k);
		}
		newFormat.push_back(make_pair(tmpPath, i.second));
	}
	if (init()) {
		cout << "Established Connection" << endl;
	}else {
		cout << "Error establishing connection" << endl;
		exit(1);
	}
	vector<Point3d> circle;
	double cm = 0.01;
	cout << "Drawing circle" << endl;
	for (double i = 0; i < 2 * M_PI; i += M_PI / 4){
		circle.push_back(Point3d(cm*cos(i), cm*sin(i), 0));
	}
	Point3d leadin, lastPoint, source;
	double dx, dy, dist, lx, ly;
	//vector<Point3d> circle = makeCircle(newFormat[0].first[0].z);
	waitKey(0);
	cout << newFormat.size() << endl;
	for (pair<vector<Point3d>, int> i : newFormat){
		cout << i.second << ", ";
	}
	cout << endl;

	cout << "Moving through points" << endl;
	for (int i = 0; i < COLORS; i++) {
		for (unsigned int j = 0; j < newFormat.size(); j++) {
			if (newFormat.at(j).second == i) {
				// Get more paint
				if (i == 0){source = SOURCE_COLOR0;}
				else if (i == 1){source = SOURCE_COLOR1;}
				else if (i == 2){source = SOURCE_COLOR2;}
				else if (i == 3){source = SOURCE_COLOR3;}

				net_move(source + Point3d(0, 0, 0.1));
				net_move(source);
				// Move in a circle
				for (unsigned int n = 0; n < circle.size(); n++){
					net_move(source + circle.at(n));
				}
				net_move(source + Point3d(0, 0, 0.1));

				// Lead-in motion
				if (newFormat.at(j).first.size() < 2){
					leadin = newFormat.at(j).first.at(0);
				}
				else{
					dy = (newFormat.at(j).first.at(1).y - newFormat.at(j).first.at(0).y);
					dx = (newFormat.at(j).first.at(1).x - newFormat.at(j).first.at(0).x);
					dist = sqrt(dx*dx + dy*dy);
					lx = newFormat.at(j).first.at(0).x - 0.02 * (dx / dist);
					ly = newFormat.at(j).first.at(0).y - 0.02 * (dy / dist);
					leadin = Point3d(lx, ly, (newFormat.at(j).first.at(0).z + 0.01));
				}
				net_move(leadin);
				//net_move(getLeadIn(newFormat[j].first));
				
				for (Point3d k : newFormat.at(j).first) {
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
	if (contour.size() < 2){
		return contour[0];
	}
	Point3d leadin;
	double dx, dy, dist, lx, ly;
	dy = (contour[1].y - contour[0].y);
	dx = (contour[1].x - contour[0].x);
	dist = sqrt(dx*dx + dy*dy);
	lx = contour[0].x - 0.02 * (dx / dist);
	ly = contour[0].y - 0.02 * (dy / dist);
	leadin = Point3d(lx, ly, (contour[0].z + 0.02));
	return leadin;
}

vector<Point3d> makeCircle(double z){
	vector<Point3d> circle;
	double cm = 0.01;
	for (double i = 0; i < 2*M_PI; i += M_PI/4){
		circle.push_back(Point3d(cm*cos(i), cm*sin(i), z));
	}
	return circle;
}

void get_paint(Point3d paintSource){
	net_move(paintSource + Point3d(0, 0, 0.1));
	net_move(paintSource);
	net_move(paintSource + Point3d(0, 0, 0.1));
}

void swap_brush(Point3d brushDropoff, Point3d newBrush) {
	cout << "Swapping brush" << endl;
	net_move(Point3d(0.25, 0, 0.2));
	net_move(Point3d(0.25, 0.1, 0.2));
	net_move(brushDropoff + Point3d(0, 0, 0.1));
	net_move(brushDropoff);
	moveGripper(0.002);
	net_move(brushDropoff + Point3d(0, 0, 0.1));
	net_move(newBrush + Point3d(0, 0, 0.1));
	cout << "Waiting for new brush" << endl;
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
	//std::cout << "Desired pose:  x: " << pose.translation().x() << " y: " << pose.translation().y() << " z: " << pose.translation().z() << "...";

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
	//std::cout << (achieved ? "Achieved Pose" : "Failed to Achieve Pose") << std::endl;
	if (!achieved)
		std::cout << "Failed to Achieve Pose" << std::endl;
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


