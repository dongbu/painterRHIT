// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <foundCore/ecTypes.h>
#include <foundCommon/ecCoordSysXForm.h>
#include <control/ecEndEffectorSet.h>
#include <control/ecFrameEndEffector.h>
#include <control/ecManipEndEffectorPlace.h>
#include <foundCore/ecApplication.h>
#include <foundCore/ecMacros.h>
#include <manipulation/ecManipulationActionManager.h>
#include <manipulationDirector/ecManipulationScript.h>
#include <math.h>
#include <remoteCommand/ecRemoteCommand.h>
//#include <xml/ecXmlObjectReaderWriter.h>
#include <iostream>
#include <boost/bind.hpp>

#define POINT_EE_SET 0
#define FRAME_EE_SET 1
#define JOINT_CONTROL_EE_SET 0xFFFFFFFF

using namespace cv;
using namespace std;
using namespace Ec;

void net_move(Point3d p);

int _tmain(int argc, _TCHAR* argv[])
{
	
	String inputFilePath = "./test.png";
	int cannyThresh = 255;
	
	const Point3d offset = Point3d(0, 0.2, 0.1);
	const double scale = 0.0002;

	Mat greyscaleImage = imread(inputFilePath, CV_LOAD_IMAGE_GRAYSCALE);
	Mat cannyOutput;
	Canny(greyscaleImage, cannyOutput, cannyThresh, cannyThresh * 2);

	//imshow("Canny Output", cannyOutput);
	vector<vector<Point>> contours;
	findContours(cannyOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(-600, 600));

	vector<vector<Point3d>> outputPoints;
	for (vector<Point> i : contours) {
		outputPoints.push_back(vector<Point3d>());
		//cout << "new path" << endl;
		for (Point j : i) {
			Point3d k = Point3d(j.x, j.y, 0);
			k = k*scale;
			k = k + offset;
			outputPoints.back().push_back(k);
			//cout << k << endl;
		}
	}

	if (init("127.0.0.1")) cout << "Established Connection" << endl;
	else {
		cout << "Error establishing connection" << endl;
		exit(1);
	}

	cout << "Moving through points" << endl;
	for (vector<Point3d> i : outputPoints) {
		for (Point3d j : i) {
			net_move(j);
		}
	}
	cout << "Done moving" << endl;
	//imshow("Canny Output", cannyOutput);
	//waitKey();
	
	return 0;
}

void net_move(Point3d p) {
	EcCoordinateSystemTransformation pose;
	pose.setTranslationX(p.x);
	pose.setTranslationY(p.y);
	pose.setTranslationZ(p.z);
	std::cout << "Desired pose:  x: " << pose.translation().x() << " y: " << pose.translation().y() << " z: " << pose.translation().z() << "...";

	setEndEffectorSet(POINT_EE_SET); // point end effector set index
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

