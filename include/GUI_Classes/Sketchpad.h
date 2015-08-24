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
	explicit Sketchpad(int *width, int *height, Shapes *ss, CytonRunner *Ava, Webcam *W, QWidget *parent = 0);
	~Sketchpad();

	CytonRunner *Ava;
	bool connected;
	Ui::Sketchpad *ui;
	DrawWindow *cvWindow;
	CVImageWidget *translator;

private:
	void closeEvent(QCloseEvent *event);

	void getColor();
	void setupQt();
	void flood(cv::Point p);

	std::string paintingNamePath, title;
	int *width, *height;

	int robotSelected; //0: cyton, 1: ABB

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

	QDialog *kMeansForm, *cannyForm, *brushForm;
	Ui::kMeans kMeansUi;
	Ui::canny cannyUi;
	Ui::brush brushUi;
	cv::Mat savedPicture;
	std::string location;

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

	void loadPhotoClicked();
	void kMeansAdjusted();
	void cannyAdjusted();
	void editingCanceled();

	void changeSize();
	void changeSize(int *width, int *height);

	void brushChanged();
	void hideBrushUi();
	void showBrushUi();

public slots:
	void redraw();
	void newClicked();
	bool openClicked();

	void loadWebcamPicture();

	void loadPhotoCannyClicked();
	void loadPhotoKmeansClicked();

	void reset();
	void highlightShape(int index);

	void setABB();
	void setCyton();

	void connectRobot();

signals:
	void newPressed();
	void prodOtherWindows();
	void hideAll();
	void resizeSimWin(int *width, int *height);
	void save(std::string);
	void load(std::string);
	void loadRobot(std::string);
	void loadPhotoCanny(cv::Mat, int, int);
	void loadPhotoKmeans(cv::Mat, int, int);
};

#endif // SKETCHPAD_H
