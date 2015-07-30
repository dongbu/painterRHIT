#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include "stdafx.h"
#include "CVImageWidget.h"
#include "CytonRunner.h"
#include "shapes.cpp"
#include "DrawWindow.cpp"
#include "imageparser.cpp"
#include "Webcam.cpp"

namespace Ui {
class Sketchpad;
}

class Sketchpad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sketchpad(int width, int height, Shapes *ss, CytonRunner *Ava, QWidget *parent = 0);
    ~Sketchpad();
	void setWebcam(Webcam *W);

	CytonRunner *Ava;


private:
    void getColor();
    void setupQt();
	void flood(cv::Point p);

    Ui::Sketchpad *ui;
    CVImageWidget *translator;
    DrawWindow *cvWindow;
	std::string paintingName;
	int width, height;

	bool connected;
	Shapes *shapes;
	Shape *currentShape;
    PolyLine *curPolyLine;
    MyEllipse *curCircle;
    MyRectangle *curRectangle;
	PixelRegion *curPixelRegion;

	QComboBox *color;
	QSpinBox *thickness;
	std::vector<int> rgbColor;
	int prevX, prevY;

	Webcam *Web;


private slots:
    void refresh(int x, int y);
    void startNewCommand();

    void saveAsClicked();
    void saveClicked();
    void openClicked();
    void newClicked();

	void loadPhotoClicked();
	void launchWebcam();

	void connectCytonClicked();
	void connectABBClicked();
	void loadWorkspaceClicked();
	void createWorkspaceClicked();
	void startupClicked();
	void shutDownClicked();

public slots:
    void redraw();

signals:
    void prodOtherWindows();
    void load(std::string);
    void save(std::string);

};

#endif // SKETCHPAD_H
