#pragma once

#include "Sketchpad.h"
#include "ui_Sketchpad.h"

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