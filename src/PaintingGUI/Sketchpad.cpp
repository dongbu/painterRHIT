#pragma once
#include "Sketchpad.h"

using namespace cv;

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
	this->setFixedHeight(height + ui->toolBar_2->height() + ui->menubar->height() + 15);
	this->setFixedWidth(width + 20);

    //Linking opencv to Qt.
    shapes = ss;
    this->translator = new CVImageWidget(ui->widget);
    connect(translator, SIGNAL(emitRefresh(int, int)), this, SLOT(refresh(int, int)));
    this->cvWindow = new DrawWindow(height,width,"garbabe_name");
    this->cvWindow->hideWindow();

	//Image set-up
	Web = new Webcam();
	Web->setMapSize(cvWindow->grid.size().width, cvWindow->grid.size().height);

    //Drawing set-up logic
    ui->actionDraw_Line->setChecked(true); //defaults to PolyLine
    getColor(); //sets class's color
	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
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

	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
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
	} else if (ui->actionActionFill->isChecked()) {
		curPixelRegion = new PixelRegion();
		curPixelRegion->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
		this->currentShape = curPixelRegion;

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
                for (size_t i = 0; i < curPolyLine->points.size(); i++) {
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

	//DRAW PIXELREGION (fill)
	else if (ui->actionActionFill->isChecked()) {
		reset = true;
		cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid

		shapes->DrawAll(cvWindow);
		flood(Point(x, y));
	}

    //DELETE CLICK CIRCLES
    if (reset) {
        shapes->addShape(currentShape);
        startNewCommand();
		cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
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

void Sketchpad::flood(Point p) {
	Mat processed;
	processed = Mat(cvWindow->grid.size().width, cvWindow->grid.size().height, CV_64F, cvScalar(0.));
	Vec3b floodColor = cvWindow->grid.at<Vec3b>(p);
	curPixelRegion->addPoint(p.x, p.y);

	std::vector<Point> pointVec;
	pointVec.push_back(p);
	while (pointVec.size() > 0)
	{
		p = pointVec.back();
		pointVec.pop_back();
		cvWindow->grid.at<Vec3b>(p);
		Vec3b curPix = cvWindow->grid.at<Vec3b>(p);
		bool skip = false;

		if ((floodColor[0] == curPix[0] && floodColor[1] == curPix[1] && floodColor[2] == curPix[2])) {
			cvWindow->grid.at<Vec3b>(p) = curPix;
			curPixelRegion->addPoint(p.x,p.y);
		}
		else skip = true;

		if (!skip && p.y - 1 > 0 && processed.at<double>(p.x, p.y - 1) != 1) { //recurse down
			processed.at<double>(p.x, p.y - 1) = 1;
			pointVec.push_back(Point(p.x, p.y - 1));
		}
		if (!skip && p.x - 1 > 0 && processed.at<double>(p.x - 1, p.y) != 1) {	//recurse left
			processed.at<double>(p.x - 1, p.y) = 1;
			pointVec.push_back(Point(p.x - 1, p.y));
		}
		if (!skip && p.y + 1 < cvWindow->grid.size().height && processed.at<double>(p.x, p.y + 1) != 1) { //recurse up
			processed.at<double>(p.x - 1, p.y) = 1;
			pointVec.push_back(Point(p.x, p.y + 1));
		}
		if (!skip && p.x + 1 < cvWindow->grid.size().width && processed.at<double>(p.x + 1, p.y) != 1) { //recurse right
			processed.at<double>(p.x + 1, p.y) != 1;
			pointVec.push_back(Point(p.x + 1, p.y));
		}
	}
}

//Tedious functions below here
/**
 * @brief sets up the Qt ui with all buttons, actions, etc.
 */
void Sketchpad::setupQt() {
	//shape connections
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionDraw_Square);
    actionGroup->addAction(ui->actionDraw_Circle);
    actionGroup->addAction(ui->actionDraw_Line);
    actionGroup->addAction(ui->actionDraw_Filled_Circle);
    actionGroup->addAction(ui->actionDraw_Filled_Rectangle);
    actionGroup->addAction(ui->actionDraw_Filled_Polygon);
	actionGroup->addAction(ui->actionActionFill);
    ui->actionDraw_Square->setCheckable(true);
    ui->actionDraw_Circle->setCheckable(true);
    ui->actionDraw_Line->setCheckable(true);
    ui->actionDraw_Filled_Rectangle->setCheckable(true);
    ui->actionDraw_Filled_Circle->setCheckable(true);
    ui->actionDraw_Filled_Polygon->setCheckable(true);
	ui->actionActionFill->setCheckable(true);
    connect(actionGroup, SIGNAL(triggered(QAction *)), this, SLOT(startNewCommand()));

	//shape modifier creation && connections
    color = new QComboBox();
	thickness = new QSpinBox();
    QStringList colors;
    colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";
    color->addItems(colors);
    thickness->setFixedWidth(60);
    thickness->setMinimum(1);
    thickness->setMaximum(25);
    thickness->setSingleStep(1);
    thickness->setValue(4);
    ui->toolBar_2->addWidget(color);
    ui->toolBar_2->addWidget(thickness);
    connect(color, SIGNAL(currentIndexChanged(int)), this, SLOT(redraw()));
    connect(thickness, SIGNAL(valueChanged(int)), this, SLOT(redraw()));

	//load/save connections
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClicked()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openClicked()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveClicked()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsClicked()));

	//image connections
	connect(ui->actionLoad_Photo, SIGNAL(triggered()), this, SLOT(loadPhotoClicked()));
	connect(ui->actionLaunch_webcam, SIGNAL(triggered()), this, SLOT(launchWebcam()));

	//robot connections
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
	if (!Ava->connect()){
		return;
	}
	ui->menuWorkspace->setEnabled(true);
}
void Sketchpad::connectABBClicked(){
	printf("setup connection to ABB here\n");

}
void Sketchpad::loadWorkspaceClicked(){
	QFileDialog directory;
	QStringList filters;
	filters << "Text files (*.xml)";
	directory.setNameFilters(filters);
	if (directory.exec()) {
		Ava->loadWorkspace(directory.selectedFiles().at(0).toStdString());
		ui->actionStartup->setEnabled(true);
		ui->actionShutdown->setEnabled(true);
	}

}
void Sketchpad::createWorkspaceClicked(){
	Ava->createWorkspace();

	ui->actionStartup->setEnabled(true);
	ui->actionShutdown->setEnabled(true);

}
void Sketchpad::startupClicked(){
	Ava->startup();
}
void Sketchpad::shutDownClicked(){
	if (Ava->shutdown()){
		ui->actionShutdown->setDisabled(true);
		ui->actionStartup->setDisabled(true);
		ui->menuWorkspace->setDisabled(true);
	}
}

void Sketchpad::loadPhotoClicked(){
	newClicked();
	QFileDialog directory;
	QStringList filters;
	filters << "Images (*.png *.xpm *.jpg)";
	directory.setNameFilters(filters);
	if (directory.exec()) {
		cv::Mat image = imread(directory.selectedFiles().at(0).toStdString());
		cv::resize(image, cvWindow->grid, cvWindow->grid.size(), 0, 0, 1);

		ImageParserContours IPC;
		IPC.setMinContourLength(5);
		IPC.setCannyThreshold(50);
		IPC.parseImage(cvWindow->grid);
		IPC.defineShapes(shapes);

		ImageParserKmeans IPK;
		IPK.setMinPixelsInRegion(5);
		IPK.parseImage(cvWindow->grid);
		IPK.defineShapes(shapes);

		cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
		shapes->DrawAll(cvWindow); //redraw window
		translator->showImage(cvWindow->grid); //actually redraw the window
		emit prodOtherWindows();
	}
}

void Sketchpad::launchWebcam() {
	this->cvWindow->grid = Web->calibrateWebcam();

	ImageParserContours IPC;
	IPC.setMinContourLength(5);
	IPC.setCannyThreshold(50);
	IPC.parseImage(cvWindow->grid);
	IPC.defineShapes(shapes);

	ImageParserKmeans IPK;
	IPK.setMinPixelsInRegion(5);
	IPK.parseImage(cvWindow->grid);
	IPK.defineShapes(shapes);

	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
	shapes->DrawAll(cvWindow); //redraw window
	translator->showImage(cvWindow->grid); //actually redraw the window
	emit prodOtherWindows();
}
