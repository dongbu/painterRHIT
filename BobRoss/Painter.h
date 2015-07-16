#ifndef PAINTER_H
#define PAINTER_H

#include "CommandWindow.h"
#include "SketchPad.h"
#include "DrawWindow.cpp"

#include <stdio.h>
#include <iostream>

class Painter
{

public:
	Painter();
	Painter(std::string name);
	Painter(Shapes shapes);
	Painter(std::string name, Shapes shapes);

	void addShape(Shape shape);
	void setDimensions(int width, int height);
	void save();
	void load(std::string projectName, std::string projectLocation);
	void setName(std::string ProjectName);
	void setLocation(std::string ProjectLocation);
	void showGUI(bool toggle);
	void launchSimulatorWindow();
	void launchCommandWindow();

private:
	Shapes shapes, completedShapes;
	std::string ProjectName, ProjectLocation;
	int width, height;

	CommandWindow commandWin;
	Sketchpad sketch;
	DrawWindow *simWin;
};

#endif // PAINTER_H
