#ifndef WorkSpace_H
#define WorkSpace_H

#include <qstring.h>
#include <qlistwidget.h>
#include <stdio.h>
#include <Shape.h>
#include <qsize.h>

class WorkSpace
{
public:
	WorkSpace();

	QString projectName, projectLocation;
	QListWidget *list;
	int lineIndex, commandIndex, solidIndex, lineAttributeIndex, runFromAdjust, finishIndex;
	std::vector<Shape *> commandStack;
	QSize canvasSize;
	int pixelToCm;
	QString fillMethod;



};


#endif
