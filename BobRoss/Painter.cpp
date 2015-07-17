#include <Painter.h>

Painter::Painter() {
	ProjectName = "Temp";
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
	simWin = new DrawWindow(width,height,ProjectName);
	commandWin = new CommandWindow();
	commandWin->setShapes(shapes);
	sketch->setShapes(shapes);
}
Painter::Painter(std::string name) {
	ProjectName = name;
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
	simWin = new DrawWindow(width, height, ProjectName);
	commandWin = new CommandWindow();
	commandWin->setShapes(shapes);
	sketch->setShapes(shapes);
}
Painter::Painter(Shapes shapes) {
	this->shapes = shapes;
	ProjectName = "Temp";
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
	simWin = new DrawWindow(width, height, ProjectName);
	commandWin = new CommandWindow();
	commandWin->setShapes(shapes);
	printf("constructor says that command window has: %i\n", commandWin->shapes.length());
	sketch->setShapes(shapes);
}
Painter::Painter(std::string name, Shapes shapes) {
	this->shapes = shapes;
	ProjectName = name;
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
	simWin = new DrawWindow(width, height, ProjectName);
	commandWin = new CommandWindow();
	commandWin->setShapes(shapes);
	sketch->setShapes(shapes);

}
void Painter::setShapes(Shapes shapes){
	this->shapes = shapes;
	commandWin->setShapes(shapes);
	sketch->setShapes(shapes);
}
void Painter::addShape(Shape *shape) {
	this->shapes.addShape(shape);
	commandWin->setShapes(shapes);
	sketch->setShapes(shapes);
}
void Painter::setDimensions(int width, int height) {
	this->width = width;
	this->height = height;
	delete simWin;
	this->simWin = new DrawWindow(width, height, ProjectName);
	simWin->show();
}
void Painter::save() {
	std::string xml = "<?xml version=\"1.0\"?>\n";
	xml.append(shapes.getXML());
	ofstream myfile;
	myfile.open(ProjectLocation + "/" + ProjectName + ".xml");
	myfile << xml;
	myfile.close();
}
void Painter::load(std::string projectName, std::string projectLocation) {
	this->ProjectLocation = projectLocation;
	this->ProjectName = projectName;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((ProjectLocation + "/" + ProjectName + ".xml").c_str());
	printf("loading file: %s\n", (ProjectLocation + "/" + ProjectName + ".xml").c_str());
	pugi::xml_node listOfShapes = doc.child("shapes");
	shapes.parseXML(&listOfShapes);
	printf("%s\n",result.description());
	printf("shapes length: %i\n", shapes.length());
	commandWin->setShapes(shapes);
	sketch->setShapes(shapes);
}
void Painter::setName(std::string ProjectName){
	this->ProjectName = ProjectName;
	delete simWin;
	this->simWin = new DrawWindow(width, height, ProjectName);
	simWin->show();
}
void Painter::setLocation(std::string ProjectLocation){
	this->ProjectLocation = ProjectLocation;
}
void Painter::showGUI(bool toggle){
	if (toggle){
		sketch->show();
		commandWin->show();
	}
	else{
		sketch->hide();
		commandWin->hide();
	}
}
void Painter::launchSimulatorWindow(){
	this->simWin->show();
	//this->shapes.drawAll(this->simWin);
}
void Painter::launchCommandWindow(){
	commandWin->show();
}