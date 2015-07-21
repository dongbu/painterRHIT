#include <Painter.h>

/**
 * @brief default constructor
 */
Painter::Painter() {
    width = 600;
    height = 600;
	this->stuffshowing = false;
}
/**
 * @brief constructor with shapes.
 * @param shapes
 */
Painter::Painter(Shapes *shapes) {
    this->shapes = shapes;
    width = 600;
    height = 600;
	this->stuffshowing = false;
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
	this->sketch->close();
	this->commandWin->close();

	if (stuffshowing){
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
	xml.append(this->getXML());
    xml.append(shapes->getXML());
	xml.append("</robot>\n");
    ofstream myfile;
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
	this->parseXML(&canvasInfo);
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
    sketch = new Sketchpad(width, height, shapes);
    launchSimulation();

    connect(sketch, SIGNAL(load(std::string)), this, SLOT(load(std::string)));
    connect(sketch, SIGNAL(save(std::string)), this, SLOT(save(std::string)));
    connect(sketch, SIGNAL(prodOtherWindows()), commandWin, SLOT(populate()));
    connect(sketch, SIGNAL(prodOtherWindows()), logic, SLOT(shapesChanged()));
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
    logic = new RunLogic(width,height,shapes);

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
std::string Painter::getXML() { 
	std::string line;
	line = "<canvas width=\"" + std::to_string(this->width) + "\" height=\"" + std::to_string(this->height) + "\">\n";

	line.append("</canvas>\n");
	return line;
	return line;
}

/*
 * @brief parses XML information
 */
void Painter::parseXML(pugi::xml_node *canvasInfo){
	int h = canvasInfo->attribute("height").as_int();
	int w = canvasInfo->attribute("width").as_int();
	this->setDimensions(w, h);
	
}