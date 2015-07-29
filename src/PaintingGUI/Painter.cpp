#include "Painter.h"

/**
 * @brief default constructor
 */
Painter::Painter() {
    width = 600;
    height = 600;
	Ava = new CytonRunner(width,height);
	this->stuffshowing = false;
	this->shapes = new Shapes();
	Web = new Webcam();

}
/**
 * @brief constructor with shapes.
 * @param shapes
 */
Painter::Painter(Shapes *shapes) {
    this->shapes = shapes;
    width = 600;
    height = 600;
	Ava = new CytonRunner(width, height);
	this->stuffshowing = false;
	Web = new Webcam();
}
/**
 * @brief add list of shapes to painter.
 * @param inboundShapes
 */
void Painter::addShapes(Shapes *inboundShapes){
    for (int i = 0; i < inboundShapes->length(); i++) {
        this->shapes->addShape(inboundShapes->at(i));
    }
}
/**
 * @brief add single shape to painter.
 * @param inboundShape
 */
void Painter::addShape(Shape *inboundShape) {
    this->shapes->addShape(inboundShape);
}
/**
 * @brief set painter dimensions
 * @param width
 * @param height
 */
void Painter::setDimensions(int width, int height) {
    this->width = width;
    this->height = height;
	Ava = new CytonRunner(width, height);

	if (stuffshowing){
		this->sketch->close();
		this->commandWin->close();
		showGUI(true);
	}
}
/**
 * @brief save into xml.
 * @param name
 */
void Painter::save(std::string name) {
    std::string xml = "<?xml version=\"1.0\"?>\n";
	xml.append("<robot>\n");
	xml.append(this->getXMLDim());
	xml.append(this->getXMLWeb());
    xml.append(shapes->getXML());
	xml.append("</robot>\n");
    std::ofstream myfile;
    myfile.open(name);
    myfile << xml;
    myfile.close();
}

/**
 * @brief load from xml.
 * @param projectLocation
 */
void Painter::load(std::string projectLocation) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file((projectLocation).c_str());
    pugi::xml_node listOfShapes = doc.child("robot").child("shapes");
    shapes->parseXML(&listOfShapes);
	pugi::xml_node canvasInfo = doc.child("robot").child("canvas");
	pugi::xml_node webcamInfo = doc.child("robot").child("zoom");
	this->parseXML(&canvasInfo,&webcamInfo);
    printf("%s\n", result.description());
    this->sketch->redraw();
}

//Functions pertaining to GUI are below//
/**
 * @brief display the GUI
 * @param toggle
 */
void Painter::showGUI(bool toggle){
	stuffshowing = toggle;
    sketch = new Sketchpad(width, height, shapes, Ava);
	sketch->setWebcam(this->Web);
    launchSimulation();

    connect(sketch, SIGNAL(load(std::string)), this, SLOT(load(std::string)));
    connect(sketch, SIGNAL(save(std::string)), this, SLOT(save(std::string)));
    connect(sketch, SIGNAL(prodOtherWindows()), commandWin, SLOT(populate()));
    connect(sketch, SIGNAL(prodOtherWindows()), logic, SLOT(shapesChanged()));
	connect(sketch, SIGNAL(sendRobot(CytonRunner*)), commandWin, SLOT(recieveRobot(CytonRunner*)));
    connect(commandWin, SIGNAL(modifiedCommand()), sketch, SLOT(redraw()));

    if (toggle){
        sketch->show();
        commandWin->show();
    }
    else{
        sketch->hide();
        commandWin->hide();
    }
}
/**
 * @brief launch the simulation window.
 */
void Painter::launchSimulation(){
    commandWin = new CommandWindow(shapes);
    logic = new RunLogic(width,height,shapes,Ava);

    connect(commandWin->ui->actionBackward, SIGNAL(triggered()), logic, SLOT(backwardClicked()));
    connect(commandWin->ui->actionForward, SIGNAL(triggered()), logic, SLOT(forwardClicked()));
    connect(commandWin->ui->actionPause, SIGNAL(triggered()), logic, SLOT(pauseClicked()));
    connect(commandWin->ui->actionPlay, SIGNAL(triggered()), logic, SLOT(runClicked()));
    connect(commandWin->ui->actionStop, SIGNAL(triggered()), logic, SLOT(stopClicked()));
    connect(commandWin, SIGNAL(runFrom(int)), logic, SLOT(runFrom(int)));
    connect(commandWin, SIGNAL(runOnly(int)), logic, SLOT(runOnly(int)));
    connect(commandWin, SIGNAL(setBreakPoint(int)), logic, SLOT(toggleBreakPoint(int)));
    connect(commandWin, SIGNAL(modifiedCommand()), logic, SLOT(shapesChanged()));
    connect(logic, SIGNAL(setBreakPointColor(int, bool)), commandWin , SLOT(recieveBreakPointColor(int, bool)));
    connect(logic, SIGNAL(setRunColor(int, bool)), commandWin, SLOT(recieveRunColor(int, bool)));
    connect(logic, SIGNAL(clearRunColors()), commandWin, SLOT(recieveClearRunColors()));
}

/*
 * @brief gets XML information
*/
std::string Painter::getXMLDim() { 
	std::string line;
	line = "<canvas width=\"" + std::to_string(this->width) + "\" height=\"" + std::to_string(this->height) + "\">\n";
	line.append("</canvas>\n");
	return line;
}

std::string Painter::getXMLWeb() {
	std::string line;
	double * zoom = Web->getWebcamZoom();
	line = "<zoom x0=\"" + std::to_string(zoom[0]) + "\" y0=\"" + std::to_string(zoom[1]) + "\" x1=\"" + std::to_string(zoom[2])
		+ "\" y1=\"" + std::to_string(zoom[3]) + "\" x2=\"" + std::to_string(zoom[4]) + "\" y2=\"" + std::to_string(zoom[5])
		+ "\" x4=\"" + std::to_string(zoom[6]) + "\" y4=\"" + std::to_string(zoom[7]) + "\">\n";
	line.append("</zoom>/n");
	return line;
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
	
	this->setDimensions(w, h);
	this->Web->setWebcamZoom(x0, y0, x1, y1, x2, y2, x3, y3);
}