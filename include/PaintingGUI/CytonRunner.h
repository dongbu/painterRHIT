#pragma once
#include <QObject>
#include <QMainWindow>
#include <ui_CytonRunner.h>

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
#include "pugixml.hpp"



#include <opencv2/opencv.hpp>
namespace Ui {
	class CytonRunner;
}
class CytonRunner: public QMainWindow
{
	Q_OBJECT

public:
	CytonRunner();
	~CytonRunner();
	bool connect();
	void loadWorkspace(std::string fileLocation);
	void createWorkspace();
	void saveWorkspace();
	void startup();
	bool shutdown();
	void goToPos(double x, double y, double z);
	void raiseBrush();
	void lowerBrush();
	void getPaint(int paint_can_id);
	void drawPoint(std::pair<double, double> pt);
	void stroke(std::pair<double, double> pt1, std::pair<double, double> pt2);
	void stroke(std::vector<std::pair<double, double>> pts);

private:
	Ui::CytonRunner *ui;
	bool goToJointHome(int type);
	EcRealVector startJointPosition;
	std::vector<std::vector<double>> canvasCorners;
	std::vector<std::pair<double, std::pair<double, double>>> paint;
	std::vector<double> convert(double x, double y, double z);
	double dx, dy, dz;
	std::string brushType;
	double theta, phi, psi;
	double currentX, currentY;
	double raiseHeight;


private:
	bool isUp;
};

