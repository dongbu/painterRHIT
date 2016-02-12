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
Sketchpad::Sketchpad(int *width, int *height, Shapes *ss, CytonRunner *Ava, Webcam *W, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::Sketchpad)
{
	//setting class variables
	this->width = width;
	this->height = height;
	this->shapes = ss;
	this->Ava = Ava;
	this->Web = W;
	this->paintingNamePath = "unpathed";
	this->title = "unnamed";

	//setting up Qt's misc. toolbars & windows.
	ui->setupUi(this);
	this->setWindowTitle(("RHobart - " + title).c_str());
	setupQt();

	//Linking opencv to Qt.
	this->translator = new CVImageWidget(ui->widget);
	connect(translator, SIGNAL(emitRefresh(int, int)), this, SLOT(respondClick(int, int)));
	this->cvWindow = new DrawWindow(*width, *height, "garbage_name", 1);

	//Drawing set-up logic
	ui->actionDraw_Line->setChecked(true); //defaults to PolyLine

	this->rgbColor = paint_util::getColor(this->color->currentText().toStdString().c_str()); //sets class's color
	cvWindow->grid.setTo(cv::Scalar(255, 255, 255)); //clear the grid
	shapes->drawAll(cvWindow); //redraw the window
	translator->convertToQImage(cvWindow->grid); //actually redraw the window
	this->startNewCommand(); //prep for initial command

	//robot logic
	//ui->menuRobot->setDisabled(true);
	connected = false;
	ui->actionShutdown->setDisabled(true);
	ui->actionABB->setCheckable(true);
	ui->actionCyton->setCheckable(true);
	ui->actionConnect->setDisabled(true);
	ui->menuWorkspace->setDisabled(true);
	connect(ui->actionABB, SIGNAL(triggered()), this, SLOT(setABB()));
	connect(ui->actionCyton, SIGNAL(triggered()), this, SLOT(setCyton()));
	connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectRobot()));
	connect(Ava, SIGNAL(finishedSettingWorkspace()), this, SLOT(completeConnection()));
}

/**
* @brief sets up the Qt ui with all buttons, actions, etc.
*/
void Sketchpad::setupQt() {
	//Moving sketchpad to top right corner
	this->setFixedHeight(*height + ui->toolBar_2->height() + ui->menubar->height() + 15);
	this->setFixedWidth(*width + 20);
	QRect r = QApplication::desktop()->availableGeometry();
	this->move(r.right() - (*width + 35), r.top());

	//building kMeans dialog
	kMeansForm = new QDialog();
	kMeansUi.setupUi(kMeansForm);
	kMeansForm->setWindowTitle("kMeans Options");
	kMeansUi.ColorInput->setValidator(new QIntValidator(1, 64));
	kMeansUi.SizeInput->setValidator(new QIntValidator(1, 500));
	kMeansUi.SkipInput->setValidator(new QIntValidator(1, 100));
	kMeansUi.accept->setDisabled(true);
	connect(kMeansUi.cancel, SIGNAL(clicked()), this, SLOT(editingCanceled()));
	connect(kMeansUi.accept, SIGNAL(clicked()), this, SLOT(kMeansAdjusted()));

	//building canny dialog
	cannyForm = new QDialog();
	cannyUi.setupUi(cannyForm);
	cannyForm->setWindowTitle("Canny Options");
	cannyUi.ThresholdInput->setValidator(new QIntValidator(1, 100));
	cannyUi.LengthInput->setValidator(new QIntValidator(1, 100));
	cannyUi.SkipInput->setValidator(new QIntValidator(1, 100));
	cannyUi.accept->setDisabled(true);
	connect(cannyUi.cancel, SIGNAL(clicked()), this, SLOT(editingCanceled()));
	connect(cannyUi.accept, SIGNAL(clicked()), this, SLOT(cannyAdjusted()));

	//building brush dialog
	brushForm = new QDialog();
	brushUi.setupUi(brushForm);
	brushForm->setWindowTitle("Brush Options");
	brushUi.widthInput->setValidator(new QIntValidator(1, 30));
	brushUi.heightInput->setValidator(new QIntValidator(1, 30));
	connect(brushUi.accept, SIGNAL(clicked()), this, SLOT(brushChanged()));
	connect(brushUi.cancel, SIGNAL(clicked()), this, SLOT(hideBrushUi()));

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
	colors << "black" << "dark grey" << "medium grey" << "light grey" << "white" << "yellow" << "orange" << "red" << "purple" << "blue" << "green";
	color->addItems(colors);
	thickness->setFixedWidth(60);
	thickness->setMinimum(1);
	thickness->setMaximum(25);
	thickness->setSingleStep(1);
	thickness->setValue(4);
	ui->toolBar_2->insertWidget(ui->actionDraw_Line, color);
	ui->toolBar_2->insertWidget(ui->actionDraw_Line, thickness);
	connect(color, SIGNAL(currentIndexChanged(int)), this, SLOT(redraw()));
	connect(thickness, SIGNAL(valueChanged(int)), this, SLOT(redraw()));

	//load/save connections
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClicked()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openClicked()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveClicked()));
	connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsClicked()));

	//image connections
	connect(ui->actionLoadPhoto, SIGNAL(triggered()), this, SLOT(loadPhotoClicked()));
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

	//misc
	connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(clearWindow()));
	connect(ui->actionSet_sketch_window_size, SIGNAL(triggered()), this, SLOT(changeSize()));
	connect(ui->actionDefine_Shape, SIGNAL(triggered()), this, SLOT(showBrushUi()));
	
}

/**
 * @brief deconstructor
 */
Sketchpad::~Sketchpad()
{
	delete ui;
}

void Sketchpad::closeEvent(QCloseEvent *event) {
	bool unsaved = this->windowTitle().at(this->windowTitle().length() - 1) == "*";
	if (!unsaved) {
		this->close();
	}
	else {
		QMessageBox::StandardButton dialog;
		dialog = QMessageBox::warning(this, "close warning", "You have unsaved work.  Do you still want to close?",
			QMessageBox::Yes | QMessageBox::No);
		if (dialog == QMessageBox::Yes) { this->close(); }
		else { event->ignore(); }
	}
}

//redraws everything on the grid.
void Sketchpad::redraw() {
	this->rgbColor = paint_util::getColor(this->color->currentText().toStdString().c_str()); //sets class's color
	startNewCommand();
	cvWindow->clearWindow(255, 255, 255);
	shapes->drawAll(cvWindow); //redraw the window
	translator->convertToQImage(cvWindow->grid); //actually redraw the window
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
void Sketchpad::respondClick(int x, int y) {
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
			translator->convertToQImage(cvWindow->grid);
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
			translator->convertToQImage(cvWindow->grid);
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
		cvWindow->clearWindow(255, 255, 255);

		shapes->drawAll(cvWindow);
		flood(Point(x, y));
	}

	//DELETE CLICK CIRCLES
	if (reset) {
		shapes->addShape(currentShape);
		startNewCommand();
		cvWindow->clearWindow(255, 255, 255);
		shapes->drawAll(cvWindow); //redraw window
		this->setWindowTitle(("RHobart - " + title + "*").c_str());
		emit prodOtherWindows();
	}
	else {
		currentShape->draw(cvWindow);
		prevX = x; //iterate
		prevY = y; //iterate
	}

	translator->convertToQImage(cvWindow->grid); //actually redraw the window

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

		if (!skip && p.y - 1 >= 0 && processed.at<double>(p.x, p.y - 1) != 1) { //down
			pointVec.push_back(Point(p.x, p.y - 1));
			processed.at<double>(p.x, p.y - 1) = 1;
		}
		if (!skip && p.x - 1 >= 0 && processed.at<double>(p.x - 1, p.y) != 1) {	//left
			pointVec.push_back(Point(p.x - 1, p.y));
			processed.at<double>(p.x - 1, p.y) = 1;
		}
		if (!skip && p.y + 1 < cvWindow->grid.size().height && processed.at<double>(p.x, p.y + 1) != 1) { //up
			pointVec.push_back(Point(p.x, p.y + 1));
			processed.at<double>(p.x, p.y + 1) = 1;

		}
		if (!skip && p.x + 1 < cvWindow->grid.size().width && processed.at<double>(p.x + 1, p.y) != 1) { //right
			pointVec.push_back(Point(p.x + 1, p.y));
			processed.at<double>(p.x + 1, p.y) = 1;
		}
	}
	curPixelRegion->addPoint(p2.x + 1, p2.y + 1);
}

void Sketchpad::loadPhotoKmeansClicked(){ kMeansForm->show();}

void Sketchpad::loadPhotoCannyClicked(){ cannyForm->show();}

void Sketchpad::loadPhotoClicked() {
	QFileDialog directory;
	QStringList filters;
	filters << "Images (*.png *.xpm *.jpg)";
	directory.setNameFilters(filters);
	if (directory.exec()) {
		cannyUi.accept->setDisabled(false);
		kMeansUi.accept->setDisabled(false);
		imageLocation = directory.selectedFiles().at(0).toStdString();
		cv::Mat temp = cv::imread(imageLocation);

		int widthDim = temp.size().width;
		int heightDim = temp.size().height;
		double scale = 1.0;
		if (temp.size().width > temp.size().height && temp.size().width > 800) { scale = widthDim / 800.0; }
		if (temp.size().height > temp.size().width && temp.size().height > 800) { scale = heightDim / 800.0; }
		widthDim = widthDim / scale;
		heightDim = heightDim / scale;
		changeSize(&widthDim, &heightDim);

		cv::resize(temp, cvWindow->grid, cvWindow->grid.size(), 0, 0, 1);
		cv::resize(temp, savedPicture, cvWindow->grid.size(), 0, 0, 1);
		this->setWindowTitle(("RHobart - " + title + "*").c_str());
		translator->convertToQImage(cvWindow->grid);
	}
}

void Sketchpad::editingCanceled() {
	kMeansForm->hide();
	cannyForm->hide();
}

void Sketchpad::kMeansAdjusted() {
	kMeansUi.accept->setDisabled(true);
	editingCanceled();
	this->setWindowTitle(("RHobart - " + title + "*").c_str());

	if (imageLocation == "") return; //make sure they didn't click "ok" with no image.
	int colorCount = kMeansUi.ColorInput->text().toInt();
	if (colorCount == 0) colorCount = 1; //don't let them have 0 colors.
	int minPixel = kMeansUi.SizeInput->text().toInt();
	if (minPixel == 0) minPixel = 1; //don't let them have empty regions.
	int skipFreq = kMeansUi.SkipInput->text().toInt();
	if (skipFreq <= 0){ skipFreq = 1; } //no
	this->redraw();
	emit prodOtherWindows();

	printf("loading photo kMeans\n");
	emit loadPhotoKmeans(savedPicture, colorCount, minPixel, skipFreq);
	printf("finished loading photo kMeans\n");
	kMeansUi.accept->setDisabled(false);
}

void Sketchpad::cannyAdjusted() {
	cannyUi.accept->setDisabled(true);
	editingCanceled();
	this->setWindowTitle(("RHobart - " + title + "*").c_str());

	if (imageLocation == "") return; //make sure they didn't click "ok" with no image.
	int minLength = cannyUi.LengthInput->text().toInt();
	if (minLength == 0) minLength = 1; //don't let them have dimensionless lines.
	int threshold = cannyUi.ThresholdInput->text().toInt();
	if (threshold == 0) threshold = 1; //don't let them have 0% threshold.
	int skipFreq = cannyUi.SkipInput->text().toInt();
	if (skipFreq <= 0){ skipFreq = 1; } //no
	this->redraw();
	emit prodOtherWindows();

	printf("loading photo canny\n");
	emit loadPhotoCanny(savedPicture, threshold, minLength, skipFreq);
	printf("finished loading photo canny\n");
	cannyUi.accept->setDisabled(false);
}

//clicking yields resizing.
void Sketchpad::calibrateWebcam() {
	printf("switch focus to the \"calibrate webcam\" window.\n");
	printf("Click the calibration window, then press 1 - 4 to set the four corners.");
	Web->calibrateWebcam();
}

//show webcam.
void Sketchpad::viewWebcam() { Web->showWebcam(); }

//change webcam.
void Sketchpad::switchWebcam() { Web->switchWebcam(); }

//judge via webcam.
void Sketchpad::judgeWebcam() { Web->judge(this->cvWindow->grid); }

//load picture from webcam.
void Sketchpad::loadWebcamPicture() {
	cv::Mat temp = Web->getWebcamSnap(cvWindow->grid);
	if (temp.size().height == 1 && temp.size().width == 1) { return; }
	cannyUi.accept->setDisabled(false);
	kMeansUi.accept->setDisabled(false);
	cv::resize(temp, savedPicture, cvWindow->grid.size(), 0, 0, 1);
	cv::resize(temp, cvWindow->grid, cvWindow->grid.size(), 0, 0, 1);
	this->setWindowTitle(("RHobart - " + title + "*").c_str());
	translator->convertToQImage(cvWindow->grid);
	cannyUi.accept->setDisabled(false);
	kMeansUi.accept->setDisabled(false);
	imageLocation = "not nothing";
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
		QString temp = saveDirectory.selectedFiles().at(0);
		temp.chop(4);
		title = temp.split("/").back().toStdString();
		this->setWindowTitle(("RHobart - " + title).c_str());
	}
}
/**
 * @brief save functionality.
 */
void Sketchpad::saveClicked() {
	if (paintingNamePath == "unpathed") saveAsClicked();
	else {
		emit save(paintingNamePath);
		this->setWindowTitle(("RHobart - " + title).c_str());
	}
}

/**
 * @brief open functionality.
 */
bool Sketchpad::openClicked() {
	bool unsaved = this->windowTitle().at(this->windowTitle().length() - 1) == "*";
	QMessageBox *m = new QMessageBox();
	m->setText("Are you sure?");
	m->setInformativeText("All unsaved changes will be lost");
	m->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	if (m->exec() != QMessageBox::Yes){ return false; }
	QFileDialog directory;
	QStringList filters;
	filters << "Text files (*.xml)";
	directory.setNameFilters(filters);
	if (directory.exec()) {
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file((directory.selectedFiles().at(0).toStdString()).c_str());
		if (!doc.child("VirtualData").empty()){
			newClicked();
			emit load(directory.selectedFiles().at(0).toStdString());
			emit prodOtherWindows();
			this->paintingNamePath = directory.selectedFiles().at(0).toStdString();
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}
/**
 * @brief New project functionality.
 */
void Sketchpad::newClicked(){
	bool unsaved = this->windowTitle().at(this->windowTitle().length() - 1) == "*";
	QMessageBox *m = new QMessageBox();
	m->setText("Are you sure?");
	m->setInformativeText("All unsaved changes will be lost");
	m->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	if (m->exec() != QMessageBox::Yes){ return; }

	cannyUi.accept->setDisabled(true);
	kMeansUi.accept->setDisabled(true);
	this->shapes->clear();
	this->paintingNamePath = "unpathed";
	this->setWindowTitle("RHobart - untitled");
	this->redraw();
}

//turn into clear button
void Sketchpad::clearWindow() {
	QMessageBox *m = new QMessageBox();
	m->setText("Are you sure?");
	m->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	if (m->exec() == QMessageBox::Yes){
		this->shapes->clear();
		this->paintingNamePath = "unpathed";
		this->redraw();
		emit prodOtherWindows();
		emit hideAll();
	}
}

//connects to the cyton and brings up a dialog to load a workspace.
void Sketchpad::loadWorkspaceClicked(){

	QFileDialog directory;
	QStringList filters;
	filters << "Text files (*.xml)";
	directory.setNameFilters(filters);

	if (directory.exec() == 0) return;
	emit loadRobot(directory.selectedFiles().at(0).toStdString());
}

//connects to the cyton and tells it to create a workspace.
void Sketchpad::createWorkspaceClicked(){
	Ava->createWorkspace();
}

//helper method to assist in creating workspaces
void Sketchpad::completeConnection() {
	Ava->startup();
	connected = true;
}

//tells the cyton to shut down.
void Sketchpad::shutDownClicked(){
	if (Ava->shutdown()){
		ui->actionShutdown->setDisabled(true);
		connected = false;
	}
}

//used to highlight a selected shape.
void Sketchpad::highlightShape(int index) {
	//printf("highlight shape has been called at index %i\n", index);
	if (index == -1) { return; }
	cvWindow->clearWindow(255, 255, 255);
	shapes->drawAll(cvWindow); //redraw window

	DrawWindow Woverlay = DrawWindow(*width, *height, "overlay", 1);
	Woverlay.clearWindow(0, 0, 0);
	shapes->drawOne(&Woverlay, index + 1, 250, 0, 200); // r,g,b should not be 0,0,0
	cvWindow->overlay(&Woverlay, 1);

	translator->convertToQImage(cvWindow->grid); //actually redraw the window
}

void Sketchpad::setABB(){
	ui->actionCyton->setChecked(false);
	if (ui->actionABB->isChecked()){
		ui->actionConnect->setEnabled(true);
		robotSelected = 1;
	}
	else{
		ui->actionConnect->setDisabled(true);
		robotSelected = -1;
	}

}

void Sketchpad::setCyton(){
	ui->actionABB->setChecked(false);
	if (ui->actionCyton->isChecked()){
		ui->actionConnect->setEnabled(true);
		robotSelected = 0;
	}
	else{
		ui->actionConnect->setDisabled(true);
		robotSelected = -1;
	}
}

void Sketchpad::connectRobot(){
	if (robotSelected == 1){
		printf("connect to ABB\n");
	}
	else if (robotSelected == 0){
		if (!Ava->connected){
			QMessageBox *m = new QMessageBox();
			m->setInformativeText("Please ensure that CytonViewer.exe is running before continuing.");
			m->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			if (m->exec() != QMessageBox::Ok) return;

			if (Ava->connect()){
				ui->menuWorkspace->setEnabled(true);
				ui->actionConnect->setText("Disconnect");
				ui->menuSelect_Robot->setDisabled(true);
			}
		}
		else{
			if (Ava->shutdown()){
				ui->menuWorkspace->setDisabled(true);
				ui->actionConnect->setText("Connect");
				ui->menuSelect_Robot->setEnabled(true);
			}
		}
	}
}

void Sketchpad::changeSize(){
	bool xOk, yOk;
	int x = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"), tr("Width (pixels):"), 600, 10, 800, 10, &xOk);
	if (!xOk) { return; }
	int y = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"), tr("Height (pixels):"), 600, 10, 800, 10, &yOk);
	if (!yOk){ return; }
	this->width = new int(x);
	this->height = new int(y);
	changeSize(this->width, this->height);
}

void Sketchpad::changeSize(int *width, int *height) {
	this->setFixedHeight(*height + ui->toolBar_2->height() + ui->menubar->height() + 15);
	this->setFixedWidth(*width + 20);
	QRect r = QApplication::desktop()->availableGeometry();
	this->move(r.right() - (*width + 35), r.top());

	this->cvWindow = new DrawWindow(*width, *height, title, 1);
	redraw();
	emit resizeSimWin(width, height);
}


void Sketchpad::brushChanged() {
	int w = brushUi.heightInput->text().toInt();
	int h = brushUi.widthInput->text().toInt();
	std::string brushType = brushUi.shapeInput->currentText().toStdString();
	Ava->curBrush = new Brush(w, h, brushType);
}

void Sketchpad::hideBrushUi() { brushForm->hide(); }
void Sketchpad::showBrushUi() { brushForm->show();  }