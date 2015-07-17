#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include "shapes.cpp"
#include "CVImageWidget.h"
#include "DrawWindow.cpp"

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

    Ui::Sketchpad *ui;
    CVImageWidget *translator;
	DrawWindow *cvWindow;
    DrawWindow *display;
    QComboBox *color;
    QSpinBox *thickness;
    Shapes *shapes;

	Shape *currentShape;
	PolyLine *curPolyLine;
	Ellipse *curCircle;
	Rectangle *curRectangle;
	int prevX, prevY;
	std::vector<int> rgbColor;

private slots:
	void refresh(int x, int y);
	void startNewCommand();

    void saveAsClicked();
    void saveClicked();
    void openClicked();
    void newClicked();

public slots:
	void redraw();

signals:
	void shapeAdded();

};

#endif // SKETCHPAD_H


