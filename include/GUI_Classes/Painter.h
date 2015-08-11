#ifndef PAINTER_H
#define PAINTER_H
#include "stdafx.h"

#include "CommandWindow.h"
#include "SketchPad.h"
#include "RunLogic.h"
#include "imageparser.cpp"

class Painter : public QObject

{
	Q_OBJECT

public:
    Painter();
    Painter(Shapes *shapes);

	Sketchpad *sketch;
	Webcam *Web;

    void addShape(Shape *inboundShape);
    void addShapes(Shapes *inboundShapes);
    void setDimensions(int width, int height);
    void showGUI(bool toggle);
    void launchSimulation();

private:
    Shapes *shapes;
    int width, height;
	bool stuffshowing;

    CommandWindow *commandWin;
    RunLogic *logic;
	CytonRunner *Ava;

	std::string Painter::getXMLDim();
	std::string Painter::getXMLWeb();

	void parseXML(pugi::xml_node *canvasInfo, pugi::xml_node *webcamInfo);

public slots:
    void save(std::string projectLocation);
    void load(std::string projectLocation);
	void loadRobot(std::string robotLocation);
	void loadPhoto(std::string photoLocation);
	void loadPhotoCanny(std::string photoLocation, int threshold, int min_line_length);
	void loadPhotoKmeans(std::string photoLocation, int colorCount, int minRegionSize);
	void loadPhotoCanny(cv::Mat img, int threshold, int min_line_length);
	void loadPhotoKmeans(cv::Mat img, int colorCount, int minRegionSize);
	void destroyAll();

};

#endif // PAINTER_H
