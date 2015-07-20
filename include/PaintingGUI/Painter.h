#ifndef PAINTER_H
#define PAINTER_H

#include "CommandWindow.h"
#include "SketchPad.h"
#include "RunLogic.h"

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
    void launchSimulation();


private:
    Shapes *shapes;
    int width, height;
	bool stuffshowing;

    CommandWindow *commandWin;
    Sketchpad *sketch;
    RunLogic *logic;

	std::string Painter::string_format(const std::string fmt, ...);
	std::string Painter::getXML();
	void parseXML(pugi::xml_node *canvasInfo);

public slots:
    void save(std::string projectLocation);
    void load(std::string projectLocation);
};

#endif // PAINTER_H
