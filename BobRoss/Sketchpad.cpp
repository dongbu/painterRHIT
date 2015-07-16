#pragma once

#include "Sketchpad.h"
#include "ui_Sketchpad.h"
#include <QMouseEvent>
#include <qvBoxLayout>

///Public methods below here
Sketchpad::Sketchpad(int width, int height, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sketchpad)
{
    ui->setupUi(this);
	this->translator = new CVImageWidget(ui->widget);
	connect(translator, SIGNAL(emitRefresh(int, int)), this, SLOT(refresh(int, int)));
	this->cvWindow = new DrawWindow(height,width,"garbage name");
	cvWindow->clearWindow(255, 255, 255);
	this->cvWindow->hideWindow();
	translator->showImage(cvWindow->grid);
}

Sketchpad::~Sketchpad()
{
    delete ui;
}

void Sketchpad::setShapes(Shapes shapes) {
	this->shapes = shapes;
}

///private methods below here///
void Sketchpad::refresh(int x, int y) {
	cvWindow->setPenColor(200, 200, 200);
	cvWindow->drawCircle(cvPoint(x, y), 5);
	translator->showImage(cvWindow->grid);
};

///slots below here///
void Sketchpad::saveAsClicked() {
}
void Sketchpad::saveClicked() {
}
void Sketchpad::openClicked() {
}
void Sketchpad::newClicked() {
}