#pragma once
#include "Sketchpad.h"

using namespace cv;

/**
 * @brief constructor
 * @param width
 * @param height
 * @param ss
 * @param W
 * @param parent
 */
Sketchpad::Sketchpad(int width, int height, Shapes *ss, CytonRunner *Ava, Webcam *W, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::Sketchpad)
{
	//settting class variables
	this->width = width;
	this->height = height;
	this->shapes = ss;
	this->Ava = Ava;
	this->Web = W;
	this->paintingNamePath = "unpathed";

	//setting up Qt's misc. toolbars & windows.
	ui->setupUi(this);
	setupQt();

	//Linking opencv to Qt.
	this->translator = new CVImageWidget(ui->widget);
	connect(translator, SIGNAL(emitRefresh(int, int)), this, SLOT(refresh(int, int)));
	this->cvWindow = new DrawWindow(width, height, "garbabe_name", 1);

	//Drawing set-up logic
	ui->actionDraw_Line->setChecked(true); //defaults to PolyLine
	getColor(); //sets class's color
	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
	shapes->drawAll(cvWindow); //redraw the window
	translator->showImage(cvWindow->grid); //actually redraw the window
	this->startNewCommand(); //prep for initial command

	//robot logic
	//ui->menuRobot->setDisabled(true);
	connected = false;
	ui->actionShutdown->setDisabled(true);
	connect(Ava, SIGNAL(finishedSettingWorkspace()), this, SLOT(completeConnection()));
}

/**
* @brief sets up the Qt ui with all buttons, actions, etc.
*/
void Sketchpad::setupQt() {
	this->setFixedHeight(height + ui->toolBar_2->height() + ui->menubar->height() + 15);
	this->setFixedWidth(width + 20);
	QRect r = QApplication::desktop()->availableGeometry();
	this->move(r.right() - (width + 35), r.top());

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
	connect(ui->actionLoad_Photo_Canny, SIGNAL(triggered()), this, SLOT(loadPhotoCannyClicked()));
	connect(ui->actionLoad_Photo_Kmeans, SIGNAL(triggered()), this, SLOT(loadPhotoKmeansClicked()));
	connect(ui->actionCalibrate, SIGNAL(triggered()), this, SLOT(calibrateWebcam()));
	connect(ui->actionView, SIGNAL(triggered()), this, SLOT(viewWebcam()));
	connect(ui->actionSwitch, SIGNAL(triggered()), this, SLOT(switchWebcam()));
	connect(ui->actionJudge, SIGNAL(triggered()), this, SLOT(judgeWebcam()));
	connect(ui->actionSnap_Webcam_Picture, SIGNAL(triggered()), this, SLOT(loadWebcamPicture()));

	//robot connections
	connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadWorkspaceClicked()));
	connect(ui->actionCreate, SIGNAL(triggered()), this, SLOT(createWorkspaceClicked()));
	connect(ui->actionShutdown, SIGNAL(triggered()), this, SLOT(shutDownClicked()));
}

/**
 * @brief deconstructor
 */
Sketchpad::~Sketchpad()
{
	delete ui;
}

//redraws everything on the grid.
void Sketchpad::redraw() {
	getColor();
	startNewCommand();

	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
	shapes->drawAll(cvWindow); //redraw the window
	translator->showImage(cvWindow->grid); //actually redraw the window
}

//begin a new command on the sketchpad.
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
		curCircle = new EllipseShape();
		if (ui->actionDraw_Filled_Circle->isChecked()) curCircle->setFill(1);
		curCircle->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
		curCircle->setThickness(thickness->text().toInt());
		this->currentShape = curCircle;
	}
	else if (ui->actionDraw_Square->isChecked() || ui->actionDraw_Filled_Rectangle->isChecked()) {
		curRectangle = new RectangleShape();
		if (ui->actionDraw_Filled_Rectangle->isChecked()) curRectangle->setFill(1);
		curRectangle->setPenColor(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
		curRectangle->setThickness(thickness->text().toInt());
		this->currentShape = curRectangle;
	}
	else if (ui->actionActionFill->isChecked()) {
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
					pp->addPoint(curPolyLine->points.at(i).x, curPolyLine->points.at(i).y);
				}
				pp->addPoint(x, y);
				this->currentShape = pp;
			}
		}
		else { //continue prev. lines
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
		}
		else { //second point clicked
			int radius = sqrt((x - prevX)*(x - prevX) + (y - prevY)*(y - prevY));
			curCircle->setData(prevX, prevY, 2 * radius);
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
		}
		else { //second point clicked
			curRectangle->setCorners(x, y, prevX, prevY);
			reset = true;
		}
	}

	//DRAW PIXELREGION (fill)
	else if (ui->actionActionFill->isChecked()) {
		reset = true;
		cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid

		shapes->drawAll(cvWindow);
		flood(Point(x, y));
	}

	//DELETE CLICK CIRCLES
	if (reset) {
		shapes->addShape(currentShape);
		startNewCommand();
		cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
		shapes->drawAll(cvWindow); //redraw window
		emit prodOtherWindows();
	}
	else {
		currentShape->draw(cvWindow);
		prevX = x; //iterate
		prevY = y; //iterate
	}

	translator->showImage(cvWindow->grid); //actually redraw the window

}

//fills in a region (essentially the bucket from MS Paint)
void Sketchpad::flood(Point p) {
	Point p2 = p;
	Mat processed;
	processed = Mat(cvWindow->grid.size().width, cvWindow->grid.size().height, CV_64F, cvScalar(0.));
	Vec3b floodColor = cvWindow->grid.at<Vec3b>(p);

	std::vector<Point> pointVec;
	pointVec.push_back(p);
	while (pointVec.size() > 0)
	{
		p = pointVec.back();
		pointVec.pop_back();
		Vec3b curPix = cvWindow->grid.at<Vec3b>(p);
		bool skip = false;

		if (floodColor[0] == curPix[0] && floodColor[1] == curPix[1] && floodColor[2] == curPix[2]) {
			curPixelRegion->addPoint(p.x, p.y);
		}
		else skip = true;

		if (!skip && p.y - 1 >= 0 && processed.at<double>(p.x, p.y - 1) != 1) { //recurse down
			pointVec.push_back(Point(p.x, p.y - 1));
			processed.at<double>(p.x, p.y - 1) = 1;
		}
		if (!skip && p.x - 1 >= 0 && processed.at<double>(p.x - 1, p.y) != 1) {	//recurse left
			pointVec.push_back(Point(p.x - 1, p.y));
			processed.at<double>(p.x - 1, p.y) = 1;
		}
		if (!skip && p.y + 1 < cvWindow->grid.size().height && processed.at<double>(p.x, p.y + 1) != 1) { //recurse up
			pointVec.push_back(Point(p.x, p.y + 1));
			processed.at<double>(p.x, p.y + 1) = 1;

		}
		if (!skip && p.x + 1 < cvWindow->grid.size().width && processed.at<double>(p.x + 1, p.y) != 1) { //recurse right
			pointVec.push_back(Point(p.x + 1, p.y));
			processed.at<double>(p.x + 1, p.y) = 1;
		}
	}
	curPixelRegion->addPoint(p2.x + 1, p2.y + 1);

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
	}
	else if (col == "orange") {
		toReplace.push_back(30);
		toReplace.push_back(144);
		toReplace.push_back(255);
	}
	else if (col == "yellow") {
		toReplace.push_back(0);
		toReplace.push_back(255);
		toReplace.push_back(255);
	}
	else if (col == "green") {
		toReplace.push_back(34);
		toReplace.push_back(139);
		toReplace.push_back(34);
	}
	else if (col == "red") {
		toReplace.push_back(34);
		toReplace.push_back(34);
		toReplace.push_back(178);
	}
	else if (col == "blue") {
		toReplace.push_back(255);
		toReplace.push_back(144);
		toReplace.push_back(30);
	}
	else if (col == "purple") {
		toReplace.push_back(240);
		toReplace.push_back(32);
		toReplace.push_back(160);
	}
	this->rgbColor = toReplace;
}

//allows you to load a file into the sketchpad window.
//these pictures are converted automatically to strokes.

void Sketchpad::loadPhotoCannyClicked(std::string loc){
	int threshold = QInputDialog::getInt(this, "Load Canny", "Threshold (%)", 50, 1, 100);
	if (!threshold) { return; }
	int min_line_length = QInputDialog::getInt(this, "Load Canny", "Minimum Line Length (pixels)", 5, 1, 100);
	if (!min_line_length) { return; }

	if (loc == ""){
		QFileDialog directory;
		QStringList filters;
		filters << "Images (*.png *.xpm *.jpg)";
		directory.setNameFilters(filters);
		if (directory.exec()) emit loadPhotoCanny(directory.selectedFiles().at(0).toStdString(), threshold, min_line_length);
	}
	else{
		emit loadPhotoCanny(loc, threshold, min_line_length);
	}
}

void Sketchpad::loadPhotoKmeansClicked(std::string loc){
	int colorCount = QInputDialog::getInt(this, "Load kMeans", "Color Count", 2, 2, 64);
	if (!colorCount) { return; }
	int minRegionSize = QInputDialog::getInt(this, "Load kMeans", "Minimum regions size (pixels)", 5, 1, 500);
	if (!minRegionSize) { return; }

	if (loc == ""){
		QFileDialog directory;
		QStringList filters;
		filters << "Images (*.png *.xpm *.jpg)";
		directory.setNameFilters(filters);
		if (directory.exec()) emit loadPhotoKmeans(directory.selectedFiles().at(0).toStdString(), colorCount, minRegionSize);
	}
	else{
		emit loadPhotoKmeans(loc, colorCount, minRegionSize);
	}
}

//clicking yields resizing.
void Sketchpad::calibrateWebcam() { printf("switch focus to the \"calibrate webcam\" window.\n"); Web->calibrateWebcam(); }

void Sketchpad::viewWebcam() { Web->showWebcam(); }

void Sketchpad::switchWebcam() { Web->switchWebcam(); }

void Sketchpad::judgeWebcam() { Web->judge(this->cvWindow->grid); }

void Sketchpad::loadWebcamPicture() {
	cv::Mat tempMat = Web->getWebcamSnap(cvWindow->grid);
	if (tempMat.size().height == 1 && tempMat.size().width == 1) { return; }
	cvWindow->grid = tempMat;

	ImageParserKmeans IPK;
	IPK.setMinPixelsInRegion(5);
	IPK.parseImage(cvWindow->grid);
	IPK.defineShapes(shapes);

	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
	shapes->drawAll(cvWindow); //redraw window
	translator->showImage(cvWindow->grid); //actually redraw the window
	prodOtherWindows();
}

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
		paintingNamePath = saveDirectory.selectedFiles().at(0).toStdString();
		emit save(paintingNamePath);
	}
}
/**
 * @brief save functionality.
 */
void Sketchpad::saveClicked() {
	if (paintingNamePath == "unpathed") saveAsClicked();
	else emit save(paintingNamePath);
}

/**
 * @brief open functionality.
 */
bool Sketchpad::openClicked() {

	QFileDialog directory;
	QStringList filters;
	filters << "Text files (*.xml)";
	directory.setNameFilters(filters);
	if (directory.exec()) {
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file((directory.selectedFiles().at(0).toStdString()).c_str());
		if (!doc.child("robot").empty()){
			newClicked();
			emit load(directory.selectedFiles().at(0).toStdString());
			emit prodOtherWindows();
			this->paintingNamePath = directory.selectedFiles().at(0).toStdString();
			return true;
		}
		else {
			printf("the robot is empty.\n");
			return false;
		}
	}
	return false;
}
/**
 * @brief New project functionality.
 */
void Sketchpad::newClicked() {
	this->shapes->clear();
	this->paintingNamePath = "unpathed";
	this->redraw();
	emit prodOtherWindows();
}

//connects to the cyton and brings up a dialog to load a workspace.
void Sketchpad::loadWorkspaceClicked(){
	//QProcess *p = new QProcess();
	//p->start("C:/\"Program Files (x86)\"/Robai/\"Cyton Gamma 1500 Viewer_4.X\"/bin/cytonViewer.exe");

	QMessageBox *m = new QMessageBox();
	m->setInformativeText("Please ensure that CytonViewer.exe is running before continuing.");
	m->setStandardButtons(QMessageBox::Ok);
	if (m->exec() != QMessageBox::Ok) return;

	QFileDialog directory;
	QStringList filters;
	filters << "Text files (*.xml)";
	directory.setNameFilters(filters);

	if (directory.exec() == 0) return;
	emit loadRobot(directory.selectedFiles().at(0).toStdString());
}

//connects to the cyton and tells it to create a workspace.
void Sketchpad::createWorkspaceClicked(){
	QMessageBox *m = new QMessageBox();
	m->setInformativeText("Please ensure that CytonViewer.exe is running before continuing.");
	m->setStandardButtons(QMessageBox::Ok);
	if (m->exec() != QMessageBox::Ok) return;

	Ava->connect();
	Ava->createWorkspace();
}

//helper method to assist in creating workspaces
void Sketchpad::completeConnection() {
	Ava->startup();
	connected = true;
	ui->actionShutdown->setEnabled(true);
}

//tells the cyton to shut down.
void Sketchpad::shutDownClicked(){
	if (Ava->shutdown()){
		ui->actionShutdown->setDisabled(true);
		connected = false;
	}
}