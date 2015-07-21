#pragma once

#include "Sketchpad.h"
#include "ui_Sketchpad.h"
#include <QMouseEvent>
#include <qvBoxLayout>
#include <QActionGroup>
#include <cmath>
#include <QFileDialog>

///Public methods below here

/**
 * @brief constructor
 * @param width
 * @param height
 * @param ss
 * @param parent
 */
Sketchpad::Sketchpad(int width, int height, Shapes *ss, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sketchpad)
{
    //setting up Qt's misc. toolbars & windows.
    ui->setupUi(this);
    setupQt();
    this->paintingName = "unnamed";
	this->setFixedHeight(height + ui->toolBar->height() + ui->menubar->height() + 30);
	this->setFixedWidth(width + ui->toolBar_2->width() + 10);

    //Linking opencv to Qt.
    shapes = ss;
    this->translator = new CVImageWidget(ui->widget);
    connect(translator, SIGNAL(emitRefresh(int, int)), this, SLOT(refresh(int, int)));
    this->cvWindow = new DrawWindow(height,width,"garbage name");
    this->cvWindow->hideWindow();

    //Drawing set-up logic
    ui->actionDraw_Line->setChecked(true); //defaults to PolyLine
    getColor(); //sets class's color
    cvWindow->clearWindow(); //clear the window
    shapes->DrawAll(cvWindow); //redraw the window
    translator->showImage(cvWindow->grid); //actually redraw the window
    this->startNewCommand(); //prep for initial command

	//robot logic
	//ui->menuRobot->setDisabled(true);
	ui->menuWorkspace->setDisabled(true);
	ui->actionStartup->setDisabled(true);
	ui->actionShutdown->setDisabled(true);

}

/**
 * @brief deconstructor
 */
Sketchpad::~Sketchpad()
{
    delete ui;
}

void Sketchpad::redraw() {
    getColor();
    startNewCommand();

    cvWindow->clearWindow(); //clear the window
    shapes->DrawAll(cvWindow); //redraw the window
    translator->showImage(cvWindow->grid); //actually redraw the window
}

void Sketchpad::startNewCommand() {
    prevX = -10;
    prevY = -10;

    if (ui->actionDraw_Line->isChecked() || ui->actionDraw_Filled_Polygon->isChecked()) {
        curPolyLine = new PolyLine();
        curPolyLine->setThickness(thickness->text().toInt());
        curPolyLine->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
        this->currentShape = curPolyLine;
    }

    else if (ui->actionDraw_Circle->isChecked() || ui->actionDraw_Filled_Circle->isChecked()) {
        curCircle = new Ellipse();
        if (ui->actionDraw_Filled_Circle->isChecked()) curCircle->setFill(1);
        curCircle->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
        this->currentShape = curCircle;
    }
    else if (ui->actionDraw_Square->isChecked() || ui->actionDraw_Filled_Rectangle->isChecked()) {
        curRectangle = new Rectangle();
        if (ui->actionDraw_Filled_Rectangle->isChecked()) curRectangle->setFill(1);
        curRectangle->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
        this->currentShape = curRectangle;
    }
}

/**
 * @brief Updates the sketchpad based on input.
 * @param x
 * @param y
 */
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
                pp->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
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
        shapes->addShape(currentShape);
        startNewCommand();
        cvWindow->clearWindow();
        shapes->DrawAll(cvWindow); //redraw window
        emit prodOtherWindows();
    }
    else {
        currentShape->draw(cvWindow);
        prevX = x; //iterate
        prevY = y; //iterate
    }

    translator->showImage(cvWindow->grid); //actually redraw the window

}

//Tedious functions below here
/**
 * @brief sets up the Qt ui with all buttons, actions, etc.
 */
void Sketchpad::setupQt() {
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

    color = new QComboBox();
    QStringList colors;
    colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";
    color->addItems(colors);

    thickness = new QSpinBox();
    thickness->setFixedWidth(60);
    thickness->setMinimum(1);
    thickness->setMaximum(25);
    thickness->setSingleStep(1);
    thickness->setValue(4);

    ui->toolBar->addWidget(color);
    ui->toolBar->addWidget(thickness);

    connect(color, SIGNAL(currentIndexChanged(int)), this, SLOT(redraw()));
    connect(thickness, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClicked()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openClicked()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveClicked()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsClicked()));

	//robot work
	Ava = new CytonRunner();
	connect(ui->actionCyton, SIGNAL(triggered()), this, SLOT(connectCytonClicked()));
	connect(ui->actionABB, SIGNAL(triggered()), this, SLOT(connectABBClicked()));
	connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadWorkspaceClicked()));
	connect(ui->actionCreate, SIGNAL(triggered()), this, SLOT(createWorkspaceClicked()));
	connect(ui->actionStartup, SIGNAL(triggered()), this, SLOT(startupClicked()));
	connect(ui->actionShutdown, SIGNAL(triggered()), this, SLOT(shutDownClicked()));


}

/**
 * @brief transforms a text color into useable rgb code.
 */
void Sketchpad::getColor() {
    QString col = this->color->currentText();
    std::vector<int> toReplace;
    if (col == "black") {
        toReplace.push_back(0);
        toReplace.push_back(0);
        toReplace.push_back(0);
    } else if (col == "orange") {
        toReplace.push_back(30);
        toReplace.push_back(144);
        toReplace.push_back(255);
    } else if (col == "yellow") {
        toReplace.push_back(0);
        toReplace.push_back(255);
        toReplace.push_back(255);
    } else if (col == "green") {
        toReplace.push_back(34);
        toReplace.push_back(139);
        toReplace.push_back(34);
    } else if (col == "red") {
        toReplace.push_back(34);
        toReplace.push_back(34);
        toReplace.push_back(178);
    } else if (col == "blue") {
        toReplace.push_back(255);
        toReplace.push_back(144);
        toReplace.push_back(30);
    } else if (col == "purple") {
        toReplace.push_back(240);
        toReplace.push_back(32);
        toReplace.push_back(160);
    }
    this->rgbColor = toReplace;
}

//Functions primarily relating to other classes are below here
/**
 * @brief saveAs functionality.
 */
void Sketchpad::saveAsClicked() {
    QFileDialog saveDirectory;
    QStringList filters;
    saveDirectory.setAcceptMode(QFileDialog::AcceptSave);
    filters << "Text files (*.xml)";
    saveDirectory.setNameFilters(filters);
    if (saveDirectory.exec()) {
        paintingName = saveDirectory.selectedFiles().at(0).toStdString();
        emit save(paintingName);
    }
}
/**
 * @brief save functionality.
 */
void Sketchpad::saveClicked() {
    if (paintingName == "unnamed") saveAsClicked();
    else emit save(paintingName);
}

/**
 * @brief open functionality.
 */
void Sketchpad::openClicked() {
    newClicked();
    QFileDialog directory;
    QStringList filters;
    filters << "Text files (*.xml)";
    directory.setNameFilters(filters);
    if (directory.exec()) {
        emit load(directory.selectedFiles().at(0).toStdString());
        emit prodOtherWindows();
        this->paintingName = directory.selectedFiles().at(0).toStdString();
    }
}
/**
 * @brief New project functionality.
 */
void Sketchpad::newClicked() {
    this->shapes->clear();
    this->paintingName = "unnamed";
    this->redraw();
    emit prodOtherWindows();
}


void Sketchpad::connectCytonClicked(){
	printf("connect to cyton\n");
	if (!Ava->connect()){
		return;
	}
	ui->menuWorkspace->setEnabled(true);
	//ui->actionStartup->setDisabled(true);
	//ui->actionShutdown->setDisabled(true);
}
void Sketchpad::connectABBClicked(){
	printf("setup connection to ABB here\n");

}
void Sketchpad::loadWorkspaceClicked(){
	printf("load workspace\n");

}
void Sketchpad::createWorkspaceClicked(){
	printf("create workspace\n");

}
void Sketchpad::startupClicked(){
	printf("startup\n");

}
void Sketchpad::shutDownClicked(){
	printf("shut down\n");

}