#ifndef painter_H
#define painter_H


#include <qstring.h>
#include <qlistwidget.h>
#include <stdio.h>
#include <Shape.h>
#include <qsize.h>
#include <Line.h>
#include "mainwindow.h"



class painter
{
public:
	painter();
	void AddToList(Line toAdd);
	void LaunchSim();
	void setDimensions(int height, int width);
	void save();
	void load(std::string path);
	//void setRobot(robot Eliza);
	void launchRobot();
	void launchSim();
	void setGuiEditable(bool state);
	void newProject();
	void addLine(Line toAdd);

	//void AddPoints(std::vector<Line> points); //method for line
	//void setColor(Color col); //method for line
	//void setThickness(int thickness); //method for line
	//save thing?
	


	QString projectName, projectLocation;
	QListWidget *list;
	int lineIndex, commandIndex, solidIndex, lineAttributeIndex, runFromAdjust, finishIndex;
	std::vector<Shape *> commandStack;
	QSize *canvasSize;
	int pixelToCm;
	QString fillMethod;
	MainWindow mainWin;
};

#endif