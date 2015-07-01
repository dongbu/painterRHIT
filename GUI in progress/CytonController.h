#pragma once
#include <control/ecEndEffectorSet.h>
#include <controlCore/ecFrameEndEffector.h>
#include <control/ecManipEndEffectorPlace.h>
#include <foundCommon/ecCoordSysXForm.h>
#include <foundCore/ecApplication.h>
#include <foundCore/ecConstants.h>
#include <foundCore/ecMacros.h>
#include <foundCore/ecTypes.h>
#include <manipulation/ecManipulationActionManager.h>
#include <manipulationDirector/ecManipulationScript.h>
#include <remoteCommand/ecRemoteCommand.h>

#include <math.h>

#include <opencv2/opencv.hpp>


class CytonController
{
public:
	CytonController();
	~CytonController();
	bool net_move(cv::Point3d p, int endEffectorSet);
	void go_home(int pos);
	bool connect();
	cv::Point3d convert(double x, double y, double z);
	bool traceLine(double x1, double y1, double x2, double y2, bool continuous, bool prevContinuous);

private:
	double s1, s2, s3, s4, s5, s6, s7; //Start positions for each joint.
};



