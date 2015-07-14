#ifndef WorkSpace_H
#define WorkSpace_H

#include <qstring.h>
#include <qlistwidget.h>
#include <stdio.h>
#include <Shape.h>
#include <qsize.h>
#include <Line.h>


class WorkSpace
{
public:
	WorkSpace();
	void AddToList(Line toAdd);
	void LaunchSim();
	void setDimensions(int height, int width);

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
};

#endif