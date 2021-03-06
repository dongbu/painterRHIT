#ifndef PAINTER_H
#define PAINTER_H
#include "stdafx.h"

#include "CommandWindow.h"
#include "SketchPad.h"
#include "RunLogic.h"
#include "imageparser.cpp"
#include "ABBRunner.h"

class Painter : public QApplication

{
	Q_OBJECT

public:
    Painter(int argc, char* argv[]);


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
	ABBRunner *chappie;

	std::string getXMLHelper(std::string xml);
	void parseXML(pugi::xml_node *canvasInfo, pugi::xml_node *webcamInfo);
	

public slots:
    void save(std::string projectLocation);
    void load(std::string projectLocation);
	void loadRobot(std::string robotLocation);
	void loadPhotoCanny(cv::Mat image, int threshold, int min_line_length, int skip_freq);
	void loadPhotoKmeans(cv::Mat image, int colorCount, int minRegionSize, int skip_freq);
	void newClicked();
	void resize(int *width, int *height);
	void murderousRampage();
};

#endif // PAINTER_H
