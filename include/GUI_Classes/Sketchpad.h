#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include "stdafx.h"
#include "CVImageWidget.h"
#include "CytonRunner.h"
#include "shapes.cpp"
#include "DrawWindow.cpp"
#include "imageparser.cpp"
#include "Webcam.cpp"
#include "WorkspaceWizard.h"

namespace Ui {
	class Sketchpad;
}

class Sketchpad : public QMainWindow
{
	Q_OBJECT

public:
	explicit Sketchpad(int width, int height, Shapes *ss, CytonRunner *Ava, Webcam *W, QWidget *parent = 0);
	~Sketchpad();

	CytonRunner *Ava;
	bool connected;
	Ui::Sketchpad *ui;
	DrawWindow *cvWindow;
	CVImageWidget *translator;

private:
	void getColor();
	void setupQt();
	void flood(cv::Point p);

	std::string paintingNamePath;
	int width, height;

	Shapes *shapes;
	Shape *currentShape;
	PolyLine *curPolyLine;
	EllipseShape *curCircle;
	RectangleShape *curRectangle;
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

	void viewWebcam();
	void switchWebcam();
	void judgeWebcam();
	void calibrateWebcam();

	void loadWorkspaceClicked();
	void createWorkspaceClicked();
	void shutDownClicked();
	void completeConnection();

public slots:
	void redraw();
	void newClicked();
	bool openClicked();

	void loadWebcamPicture();

	void loadPhotoCannyClicked(std::string loc = "", int t = 0, int mll = 0);
	void loadPhotoKmeansClicked(std::string loc = "", int cc = 0, int mrs = 0);

signals:
	void newPressed();
	void prodOtherWindows();
	void save(std::string);
	void load(std::string);
	void loadRobot(std::string);
	void loadPhotoCanny(std::string, int, int);
	void loadPhotoKmeans(std::string, int, int);
};

#endif // SKETCHPAD_H
