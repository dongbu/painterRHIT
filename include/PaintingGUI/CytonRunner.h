#pragma once
#include "ui_CytonRunner.h"
#include "shapes.cpp"
#include "regionToPaths.cpp"
#include <QObject>
#include <QMainWindow>
#include <qwidget.h>


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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/highgui.h>

#include <math.h>
#include <qobject.h>
#include <iostream>
#include "pugixml.hpp"
#include <qdialog.h>

#include <opencv2/opencv.hpp>


namespace Ui {
	class CytonRunner;
}

class CytonRunner: public QDialog
{
	Q_OBJECT

public:
	explicit CytonRunner(QWidget *parent = 0);
	~CytonRunner();
	bool connect();
	void loadWorkspace(std::string fileLocation);
	void createWorkspace();
	void saveWorkspace();
	void startup();
	bool shutdown();
	void setSimulationSize(int width, int height);
	void goToPos(double x, double y, double z);
	void raiseBrush();
	void lowerBrush();
	void getPaint(int paint_can_id);
	void drawPoint(cv::Point pt);
	void stroke(cv::Point pt1, cv::Point pt2);
	void stroke(std::vector<cv::Point> pts);
	void paintShape(Shape *s);
	void setCanvasSize(double width, double height);

private:
	Ui::CytonRunner *ui;
	bool goToJointHome(int type);
	EcRealVector startJointPosition;
	std::vector<cv::Point3d> canvasCorners;
	std::vector<std::pair<int, cv::Point>> paint;
	std::vector<double> convert(double x, double y, double z);
	double dx, dy, dz;
	std::string brushType;
	double theta, phi, psi;
	double currentX, currentY;
	double raiseHeight;
	int width, height;
	double cWidth, cHeight;
	double xScale, yScale;

	bool isUp;
};

