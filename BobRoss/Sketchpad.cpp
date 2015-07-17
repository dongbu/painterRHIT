#pragma once

#include "Sketchpad.h"
#include "ui_Sketchpad.h"
#include <QMouseEvent>
#include <qvBoxLayout>
#include <QActionGroup>
#include <cmath>

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
	
	QActionGroup *actionGroup = new QActionGroup(this);
	actionGroup->addAction(ui->actionDraw_Square);
	actionGroup->addAction(ui->actionDraw_Circle);
	actionGroup->addAction(ui->actionDraw_Line);
	actionGroup->addAction(ui->actionDraw_Filled_Circle);
	actionGroup->addAction(ui->actionDraw_Filled_Rectangle);
	actionGroup->addAction(ui->actionDraw_Filled_Polygon);
	ui->actionDraw_Square->setCheckable(true);
	ui->actionDraw_Circle->setCheckable(true);
	ui->actionDraw_Line->setCheckable(true);
	ui->actionDraw_Filled_Rectangle->setCheckable(true);
	ui->actionDraw_Filled_Circle->setCheckable(true);
	ui->actionDraw_Filled_Polygon->setCheckable(true);
	connect(actionGroup, SIGNAL(triggered(QAction *)), this, SLOT(startNewCommand()));

	ui->actionDraw_Line->setChecked(true);
	this->startNewCommand();

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
	//DRAW CLICK CIRCLE//
	cvWindow->setPenColor(200, 200, 200);
	cvWindow->setLineThickness(2);
	cvWindow->drawCircle(cvPoint(x, y), 6);
	bool reset = false;

	//DRAW POLYLINE//
	if (ui->actionDraw_Line->isChecked() || ui->actionDraw_Filled_Polygon->isChecked()) {
		if (abs(x - prevX) < 6 && abs(y - prevY) < 6) {	//finish old line
			reset = true;
			//actually, we were drawing a polyshape
			if (ui->actionDraw_Filled_Polygon->isChecked()) {
				PolyPoints *pp = new PolyPoints();
				pp->setPenColor(200, 200, 200);
				pp->setThickness(4);
				for (int i = 0; i < curPolyLine->points.size(); i++) {
					pp->addPoint(curPolyLine->points.at(i).x,curPolyLine->points.at(i).y);
				}
				pp->addPoint(x, y);
				this->currentShape = pp;
			}
		} else { //continue prev. lines
			curPolyLine->addPoint(x, y);
		}
	}

	//DRAW CIRCLE//
	else if (ui->actionDraw_Circle->isChecked() || ui->actionDraw_Filled_Circle->isChecked()) {
		if (prevX == -10) { //first point clicked
			prevX = x;
			prevY = y;
			translator->showImage(cvWindow->grid);
			return;
		} else { //second point clicked
			int radius = sqrt((x - prevX)*(x - prevX) + (y - prevY)*(y - prevY));
			curCircle->setData(prevX, prevY, 2*radius);
			reset = true;
		}
	}
	
	//DRAW RECTANGLE
	else if (ui->actionDraw_Square->isChecked() || ui->actionDraw_Filled_Rectangle->isChecked()) {
		if (prevX == -10) { //first point clicked
			prevX = x;
			prevY = y;
			translator->showImage(cvWindow->grid);
			return;
		} else { //second point clicked
			curRectangle->setCorners(x, y, prevX, prevY);
			reset = true;
		}
	}

	//DELETE CLICK CIRCLES
	if (reset) {
		this->shapes.addShape(currentShape);
		startNewCommand();
		cvWindow->clearWindow();
		this->shapes.drawAll(cvWindow); //redraw window
	}
	else {
		currentShape->draw(cvWindow);
		prevX = x; //iterate
		prevY = y; //iterate
	}

	translator->showImage(cvWindow->grid); //actually redraw the window

}

///slots below here///

void Sketchpad::startNewCommand() {
	prevX = -10;
	prevY = -10;

	if (ui->actionDraw_Line->isChecked() || ui->actionDraw_Filled_Polygon->isChecked()) {
		curPolyLine = new PolyLine();
		curPolyLine->setThickness(4);
		curPolyLine->setPenColor(200, 200, 200);
		this->currentShape = curPolyLine;
	}
	else if (ui->actionDraw_Circle->isChecked() || ui->actionDraw_Filled_Circle->isChecked()) {
		curCircle = new Ellipse();
		if (ui->actionDraw_Filled_Circle->isChecked()) curCircle->setFill(1);
		curCircle->setPenColor(200, 200, 200);
		this->currentShape = curCircle;
	}
	else if (ui->actionDraw_Square->isChecked() || ui->actionDraw_Filled_Rectangle->isChecked()) {
		curRectangle = new Rectangle();
		if (ui->actionDraw_Filled_Rectangle->isChecked()) curRectangle->setFill(1);
		curRectangle->setPenColor(200, 200, 200);
		this->currentShape = curRectangle;
	}
}

void Sketchpad::saveAsClicked() {
}
void Sketchpad::saveClicked() {
}
void Sketchpad::openClicked() {
}
void Sketchpad::newClicked() {
}