#pragma once
#include "stdafx.h"
#include "shapes.cpp"
#include "regionToPaths.cpp"
#include "workspacewizard.h";


namespace Ui {
	class CytonRunner;
}

class CytonRunner: public QDialog
{
	Q_OBJECT

public:
	explicit CytonRunner(int width, int height);
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
	void setCanvasSize(double width, double height);
	void paintShape(Shape *s);

	volatile bool connected;

private:
	Ui::CytonRunner *ui;
	bool goToJointHome(int type);
	EcRealVector startJointPosition;
	std::vector<double> convert(double x, double y, double z);

	double dx, dy, dz;
	std::vector<cv::Point3d> canvasCorners;
	std::vector<std::pair<int, cv::Point>> paint;
	std::string brushType;
	double theta, phi, psi;
	double currentX, currentY;
	double raiseHeight;
	int width, height;
	double cWidth, cHeight;
	double xScale, yScale;

	bool isUp;

signals:
	void finishedShape();
};

