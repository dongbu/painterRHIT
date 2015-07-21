#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include "../src/PaintingGUI/shapes.cpp"
#include "CVImageWidget.h"
#include "../src/PaintingGUI/DrawWindow.cpp"
#include "CytonRunner.h"

#include <QObject>
#include <QMainWindow>
#include <vector>
#include <QComboBox>
#include <QSpinBox>
#include <QMainWindow>

namespace Ui {
class Sketchpad;
}

class Sketchpad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sketchpad(int width, int height, Shapes *ss, QWidget *parent = 0);
    ~Sketchpad();

private:
    void getColor();
    void setupQt();
	void flood(cv::Point p);

    Ui::Sketchpad *ui;
    CVImageWidget *translator;
    DrawWindow *cvWindow;
    QComboBox *color;
    QSpinBox *thickness;
    Shapes *shapes;
    Shape *currentShape;
    PolyLine *curPolyLine;
    Ellipse *curCircle;
    Rectangle *curRectangle;
	PixelRegion *curPixelRegion;
    int prevX, prevY;
    std::vector<int> rgbColor;
    std::string paintingName;
	CytonRunner *Ava;

private slots:
    void refresh(int x, int y);
    void startNewCommand();

    void saveAsClicked();
    void saveClicked();
    void openClicked();
    void newClicked();

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


