#ifndef PAINTER_H
#define PAINTER_H

#include "CommandWindow.h"
#include "SketchPad.h"
#include "DrawWindow.cpp"

#include <stdio.h>
#include <iostream>
#include <qobject.h>

class Painter : public QObject

{
	Q_OBJECT

public:
	Painter();
	Painter(Shapes *shapes);

	void addShape(Shape *inboundShape);
	void addShapes(Shapes *inboundShapes);
	void setDimensions(int width, int height);
	void setName(std::string ProjectName);
	void setLocation(std::string ProjectLocation);
	void showGUI(bool toggle);
	void launchSimulatorWindow();
	void launchCommandWindow();

	//Ask about deleting the following commented out things:
	//void load(std::string projectName, std::string projectLocation);
	//std::string ProjectName, ProjectLocation;
	//Shapes completedShapes;
	//Painter(std::string name, Shapes *shapes);
	//Painter(std::string name);
	//void setShapes(Shapes *shapes);

private:
	Shapes *shapes;
	int width, height;

	CommandWindow *commandWin;
	Sketchpad *sketch;
	DrawWindow *simWin;

public slots:
	void save(std::string projectLocation);
	void load(std::string projectLocation);
};

#endif // PAINTER_H
