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

	Sketchpad *sketch;
	Webcam *Web;

    void setDimensions(int *width, int *height);
    void showGUI();

private:
    Shapes *shapes;
    int *width, *height;
	bool stuffshowing;

    CommandWindow *commandWin;
    RunLogic *logic;
	CytonRunner *Ava;

	std::string getXMLHelper(std::string xml);
	void parseXML(pugi::xml_node *canvasInfo, pugi::xml_node *webcamInfo);

public slots:
    void save(std::string projectLocation);
    void load(std::string projectLocation);
	void loadRobot(std::string robotLocation);
	void loadPhotoCanny(cv::Mat image, int threshold, int min_line_length);
	void loadPhotoKmeans(cv::Mat image, int colorCount, int minRegionSize);
	void newClicked();
	void resize(int *width, int *height);
};

#endif // PAINTER_H
