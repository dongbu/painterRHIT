#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include "shapes.cpp"
#include "CVImageWidget.h"

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
    explicit Sketchpad(QWidget *parent = 0);
    ~Sketchpad();
    void setShapes(Shapes shapes);

private:
    Ui::Sketchpad *ui;
    CVImageWidget translator;
    DrawWindow *display;
    QComboBox *color;
    QSpinBox *Thickness;
    Shapes shapes;

private slots:
    void saveAsClicked();
    void saveClicked();
    void openClicked();
    void newClicked();

protected:
	void mousePressEvent(QMouseEvent* event);
};

#endif // SKETCHPAD_H


