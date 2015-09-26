/*
------------------------------------------------------
--Authors:  Josh Crook, Zach Dougherty, Luke Drong, Gunnar Horve, Andrew Conru
--Date:  09/26/2015
--Name:  BobRoss
--Desc:  Constructs the basic GUI framework of RHIT's RHobart contest entry.
Using Qt, creates && links 3 main windows:  Sketchpad, CommandWindow, SimWin.
------------------------------------------------------
--Change History
9/26/15:  refactored && deleted outdated methods

*/

#include "Painter.h"


/**
 * @brief default constructor
 */
Painter::Painter() {
	width = new int(600);
	height = new int(600);
	Ava = new CytonRunner(width, height);
	this->stuffshowing = false;
	this->shapes = new Shapes();
	Web = new Webcam(width, height);
}

/**
 * @brief set painter dimensions
 * @param width
 * @param height
 */
void Painter::setDimensions(int *width, int *height) {
	this->width = width;
	this->height = height;

	if (stuffshowing){
		this->sketch->close();
		this->commandWin->close();
		showGUI();
	}
}
/**
 * @brief save all appropriate info into an xml.
 * @param name
 */
void Painter::save(std::string name) {
	std::string xml = "<?xml version=\"1.0\"?>\n";
	xml.append("<VirtualData>\n");
	xml = this->getXMLHelper(xml);
	xml.append(shapes->getXML());
	xml.append("</VirtualData>\n");
	std::ofstream myfile;
	myfile.open(name);
	myfile << xml;
	myfile.close();
}

/**
 * @brief load from xml and populate appropriate params.
 * @param projectLocation
 */
void Painter::load(std::string projectLocation) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((projectLocation).c_str());
	pugi::xml_node listOfShapes = doc.child("VirtualData").child("shapes");
	shapes->parseXML(&listOfShapes);
	pugi::xml_node canvasInfo = doc.child("VirtualData").child("canvas");
	pugi::xml_node webcamInfo = doc.child("VirtualData").child("zoom");
	this->parseXML(&canvasInfo, &webcamInfo);
	this->sketch->redraw();
}

/*
* @brief gets XML save information not pertaining to shapes
*/
std::string Painter::getXMLHelper(std::string xml) {
	std::string line;
	line = "<canvas width=\"" + std::to_string(*this->width) + "\" height=\"" + std::to_string(*this->height) + "\">\n";
	line.append("</canvas>\n");
	xml.append(line);

	std::string line1;
	double * zoom = Web->getWebcamZoom();
	line1 = "<zoom x0=\"" + std::to_string(zoom[0]) + "\" y0=\"" + std::to_string(zoom[1]) + "\" x1=\"" + std::to_string(zoom[2])
		+ "\" y1=\"" + std::to_string(zoom[3]) + "\" x2=\"" + std::to_string(zoom[4]) + "\" y2=\"" + std::to_string(zoom[5])
		+ "\" x3=\"" + std::to_string(zoom[6]) + "\" y3=\"" + std::to_string(zoom[7]) + "\">\n";
	line1.append("</zoom>/n");
	xml.append(line1);
	return xml;
}

/*
* @brief parses XML information
*/
void Painter::parseXML(pugi::xml_node *canvasInfo, pugi::xml_node *webcamInfo){
	int h = canvasInfo->attribute("height").as_int();
	int w = canvasInfo->attribute("width").as_int();
	int x0 = webcamInfo->attribute("x0").as_int();
	int y0 = webcamInfo->attribute("y0").as_int();
	int x1 = webcamInfo->attribute("x1").as_int();
	int y1 = webcamInfo->attribute("y1").as_int();
	int x2 = webcamInfo->attribute("x2").as_int();
	int y2 = webcamInfo->attribute("y2").as_int();
	int x3 = webcamInfo->attribute("x3").as_int();
	int y3 = webcamInfo->attribute("y3").as_int();

	this->setDimensions(new int(w), new int(h));

	this->Web->setWebcamZoom(x0, y0, x1, y1, x2, y2, x3, y3);
}

//connect to a robot
void Painter::loadRobot(std::string robotLocation) {
	if (!Ava->loadWorkspace(robotLocation)) return;
	Ava->connect();
	Ava->startup();

	if (stuffshowing) {
		this->sketch->connected = true;
		this->sketch->ui->actionShutdown->setEnabled(true);
	}
}

//load photo canny from location.
void Painter::loadPhotoCanny(cv::Mat image, int threshold, int min_line_length){
	logic->clearSimWin(); //white
	logic->hideSimWin();

	ImageParserContours IPC;
	IPC.setMinContourLength(min_line_length);
	IPC.setCannyThreshold(threshold);
	IPC.parseImage(image);
	IPC.defineShapes(shapes);

	sketch->cvWindow->clearWindow(255, 255, 255);
	shapes->drawAll(sketch->cvWindow); //redraw window
	sketch->translator->showImage(sketch->cvWindow->grid); //actually redraw the window
	sketch->prodOtherWindows();
}

//load photo kmeans from location.
void Painter::loadPhotoKmeans(cv::Mat image, int colorCount, int minRegionSize) {
	logic->clearSimWin(); //white
	logic->hideSimWin();

	ImageParserKmeans IPK;
	IPK.setNumColors(colorCount);
	IPK.setMinPixelsInRegion(minRegionSize);
	IPK.parseImage(image);
	IPK.defineShapes(shapes);

	sketch->cvWindow->clearWindow(255, 255, 255);
	shapes->drawAll(sketch->cvWindow); //redraw window
	sketch->translator->showImage(sketch->cvWindow->grid); //actually redraw the window
	sketch->prodOtherWindows();
}

//Functions pertaining to GUI are below//
/**
 * @brief display the GUI
 * @param toggle
 */
void Painter::showGUI(){
	stuffshowing = true;
	sketch = new Sketchpad(width, height, shapes, Ava, Web);
	commandWin = new CommandWindow(shapes);
	logic = new RunLogic(*width, *height, shapes, Ava);

	//commandWindow && runLogic connections
	connect(commandWin->ui->actionBackward, SIGNAL(triggered()), logic, SLOT(backwardClicked()));
	connect(commandWin->ui->actionForward, SIGNAL(triggered()), logic, SLOT(forwardClicked()));
	connect(commandWin->ui->actionPause, SIGNAL(triggered()), logic, SLOT(pauseClicked()));
	connect(commandWin->ui->actionPlay, SIGNAL(triggered()), logic, SLOT(runClicked()));
	connect(commandWin->ui->actionClear, SIGNAL(triggered()), logic, SLOT(clearClicked()));
	connect(commandWin, SIGNAL(runFrom(int)), logic, SLOT(runFrom(int)));
	connect(commandWin, SIGNAL(runOnly(int)), logic, SLOT(runOnly(int)));
	connect(commandWin, SIGNAL(modifiedCommand()), logic, SLOT(shapesChanged()));
	connect(commandWin, SIGNAL(modeUpdated(QString, int)), logic, SLOT(updateMode(QString, int)));
	connect(logic, SIGNAL(updateCommandList(int, QString)), commandWin, SLOT(updateCommandList(int, QString)));

	//sketchpad connections
	connect(sketch, SIGNAL(save(std::string)), this, SLOT(save(std::string)));
	connect(sketch, SIGNAL(load(std::string)), this, SLOT(load(std::string)));
	connect(sketch, SIGNAL(loadRobot(std::string)), this, SLOT(loadRobot(std::string)));
	connect(sketch, SIGNAL(loadPhotoCanny(cv::Mat, int, int)), this, SLOT(loadPhotoCanny(cv::Mat, int, int)));
	connect(sketch, SIGNAL(loadPhotoKmeans(cv::Mat, int, int)), this, SLOT(loadPhotoKmeans(cv::Mat, int, int)));
	connect(sketch, SIGNAL(prodOtherWindows()), commandWin, SLOT(populate()));
	connect(sketch, SIGNAL(prodOtherWindows()), logic, SLOT(shapesChanged()));
	connect(sketch->ui->actionNew, SIGNAL(triggered()), this, SLOT(newClicked()));
	connect(sketch, SIGNAL(hideAll()), this, SLOT(newClicked()));
	connect(sketch, SIGNAL(resizeSimWin(int*, int*)), this, SLOT(resize(int*, int*)));
	connect(commandWin, SIGNAL(modifiedCommand()), sketch, SLOT(redraw()));
	connect(commandWin, SIGNAL(highlightShape(int)), sketch, SLOT(highlightShape(int)));

	sketch->show();
}

//hide all windows but sketchpad (i.e., new clicked)
void Painter::newClicked(){
	commandWin->hide();
	logic->hideSimWin();
	logic->clearClicked();
}

void Painter::resize(int *width, int *height){
	this->width = width;
	this->height = height;
	Web->map_width = width;
	Web->map_height = height;
	logic->width = *width;
	logic->height = *height;
	logic->resetStepTaken();
	logic->clearClicked();

	logic->resetSimWin(*width, *height, "Simulation Window");
	logic->showSimWin();
}