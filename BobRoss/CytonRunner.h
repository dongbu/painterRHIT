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
#include <qobject.h>
#include <iostream>

#include <opencv2/opencv.hpp>
class CytonRunner
{
public:
	CytonRunner();
	~CytonRunner();
	bool connect();
	void loadWorkspace(std::string fileLocation);
	void createWorkspace();
	void saveWorkspace();
	void startup();
	void shutdown();
	void goToPos(int x, int y);
	void raiseBrush();
	void lowerBrush();
	void getPaint(int paint_can_id);
	void drawPoint(std::pair<int, int> pt);
	void stroke(std::pair<int, int> pt1, std::pair<int, int> pt2);

	





private:
	bool isUp;
};

