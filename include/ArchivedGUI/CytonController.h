#ifndef CYTONCONTROLLER_H
#define CYTONCONTROLLER_H

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
#include <qobject.h>

#include <opencv2/opencv.hpp>


class CytonController : public QObject
{
	Q_OBJECT

public:
	CytonController();
	~CytonController();
	bool net_move(cv::Point3d p, int endEffectorSet);
	bool connect();
	cv::Point3d convert(double x, double y, double z);

private:
	double s1, s2, s3, s4, s5, s6, s7; //Start positions for each joint.

public slots:
	bool traceLine(double x1, double y1, double x2, double y2, bool continuous, bool prevContinuous, int currentEditor);
	void go_home(int pos);

signals:
	void finishedLine(int currentEditor);
};


#endif


