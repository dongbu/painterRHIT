#pragma once

#include "Sketchpad.h"
#include "ui_Sketchpad.h"
#include <QMouseEvent>

///Public methods below here
Sketchpad::Sketchpad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sketchpad)
{
    ui->setupUi(this);
}

Sketchpad::~Sketchpad()
{
    delete ui;
}

void Sketchpad::setShapes(Shapes shapes) {
	this->shapes = shapes;

}

void Sketchpad::mousePressEvent(QMouseEvent * event){
	//get mouse coordinates
	int currentX = event->localPos().x();
	int currentY = event->localPos().y();
}

///slots below here
void Sketchpad::saveAsClicked() {
}
void Sketchpad::saveClicked() {
}
void Sketchpad::openClicked() {
}
void Sketchpad::newClicked() {
}