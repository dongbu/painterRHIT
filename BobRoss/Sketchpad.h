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
    explicit Sketchpad(int width, int height, QWidget *parent = 0);
    ~Sketchpad();
    void setShapes(Shapes shapes);

private:
    Ui::Sketchpad *ui;
    CVImageWidget *translator;
	DrawWindow *cvWindow;
    DrawWindow *display;
    QComboBox *color;
    QSpinBox *Thickness;
    Shapes shapes;
	Shape *currentShape;

private slots:
	void refresh(int x, int y);

    void saveAsClicked();
    void saveClicked();
    void openClicked();
    void newClicked();


};

#endif // SKETCHPAD_H


