#pragma once
#include "stdafx.h"

#include "shapes.cpp"
#include "regionToPaths.cpp"

namespace Ui {
	class CytonRunner;
}

class CytonRunner: public QDialog
{
	Q_OBJECT


public:
	explicit CytonRunner(int *width, int *height);
	~CytonRunner();

	void changePaint(int new_paint_can_id);
	bool connect();
	volatile bool connected;
	std::vector<std::pair<int, std::pair<cv::Point2d, std::vector<int>>>> paint;
	void createWorkspace();

	Brush *curBrush;
	void decidePaint(int r, int g, int b);
	bool goToJointHome(int type);

	bool loadWorkspace(std::string fileLocation);
	void saveWorkspace();

	void startup();
	bool shutdown();

	void stroke(cv::Point pt1, cv::Point pt2);
	void stroke(std::vector<cv::Point> pts);

	bool strokeInProgress;
	void tellFinished();

private:
	void setSimulationSize(int *width, int *height);
	void goToPos(double x, double y, double z, bool toggle = false);
	void raiseBrush();
	void lowerBrush();
	void getPaint(int paint_can_id);
	void drawPoint(cv::Point pt);

	void setCanvasSize(double width, double height);
	
	int lastPaintColor;
	Ui::CytonRunner *ui;
	EcRealVector startJointPosition;
	std::vector<double> convert(double x, double y, double z);
	void regulateWorkspaceData();

	double dx, dy, dz;
	std::vector<cv::Point3d> canvasCorners;
	std::string brushType;
	double currentX, currentY;
	double raiseHeight;
	int *width, *height;
	double cWidth, cHeight;
	double xScale, yScale;

	bool isUp;

signals:
	void finishedShape();
	void finishedSettingWorkspace();
};

