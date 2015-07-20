#include <Painter.h>

/**
 * @brief default constructor
 */
Painter::Painter() {
    width = 1000;
    height = 800;
}
/**
 * @brief constructor with shapes.
 * @param shapes
 */
Painter::Painter(Shapes *shapes) {
    this->shapes = shapes;
    width = 1000;
    height = 800;
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
}
/**
 * @brief save into xml.
 * @param name
 */
void Painter::save(std::string name) {
    std::string xml = "<?xml version=\"1.0\"?>\n";
    xml.append(shapes->getXML());
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
    pugi::xml_node listOfShapes = doc.child("shapes");
    shapes->parseXML(&listOfShapes);
    printf("%s\n", result.description());
    this->sketch->redraw();
}

//Functions pertaining to GUI are below//
/**
 * @brief display the GUI
 * @param toggle
 */
void Painter::showGUI(bool toggle){
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

//Antiquated functions below here (should consult with Zach before deleting)//

//void Painter::load(std::string projectName, std::string projectLocation) {
//	this->ProjectLocation = projectLocation;
//	this->ProjectName = projectName;
//	pugi::xml_document doc;
//	pugi::xml_parse_result result = doc.load_file((ProjectLocation + "/" + ProjectName + ".xml").c_str());
//	printf("loading file: %s\n", (ProjectLocation + "/" + ProjectName + ".xml").c_str());
//	pugi::xml_node listOfShapes = doc.child("shapes");
//	shapes->parseXML(&listOfShapes);
//	printf("%s\n", result.description());
//}

//void Painter::save(std::string name) {
//	std::string xml = "<?xml version=\"1.0\"?>\n";
//	xml.append(shapes->getXML());
//	ofstream myfile;
//	myfile.open(ProjectLocation + "/" + ProjectName + ".xml");
//	myfile << xml;
//	myfile.close();
//}

//void Painter::setName(std::string ProjectName){
//	this->ProjectName = ProjectName;
//	delete simWin;
//	this->simWin = new DrawWindow(width, height, ProjectName);
//	simWin->show();
//}

//void Painter::setLocation(std::string ProjectLocation){
//	this->ProjectLocation = ProjectLocation;
//}
//Painter::Painter() {
//	ProjectName = "Temp";
//	ProjectLocation = "";
//	width = 1000;
//	height = 800;
//}

//Painter::Painter(std::string name) {
//	ProjectName = name;
//	ProjectLocation = "";
//	width = 1000;
//	height = 800;
//}

//Painter::Painter(Shapes *shapes) {
//	this->shapes = shapes;
//	ProjectName = "Temp";
//	ProjectLocation = "";
//	width = 1000;
//	height = 800;
//}

//Painter::Painter(std::string name, Shapes *shapes) {
//	this->shapes = shapes;
//	ProjectName = name;
//	ProjectLocation = "";
//	width = 1000;
//	height = 800;
//}

//void Painter::setShapes(Shapes *shapes){
//	this->shapes = shapes;
//}

//void Painter::launchSimulatorWindow(){
//	simWin = new DrawWindow(width, height, "Simulator");
//	this->simWin->show();
//	//this->shapes.drawAll(this->simWin);
//}

//void Painter::launchCommandWindow(){
//	commandWin = new CommandWindow(shapes);
//	commandWin->show();
//}
