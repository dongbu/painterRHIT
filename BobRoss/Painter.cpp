#include <Painter.h>

Painter::Painter() {
	ProjectName = "Temp";
	ProjectLocation = "";
	width = 1000;
	height = 800;

	simWin = new DrawWindow(width,height,ProjectName);
}
Painter::Painter(std::string name) {
	ProjectName = name;
	ProjectLocation = "";
	width = 1000;
	height = 800;

	simWin = new DrawWindow(width, height, ProjectName);
}
Painter::Painter(Shapes shapes) {
	this->shapes = shapes;
	ProjectName = "Temp";
	ProjectLocation = "";
	width = 1000;
	height = 800;

	simWin = new DrawWindow(width, height, ProjectName);
}
Painter::Painter(std::string name, Shapes shapes) {
	this->shapes = shapes;
	ProjectName = name;
	ProjectLocation = "";
	width = 1000;
	height = 800;

	simWin = new DrawWindow(width, height, ProjectName);

}
void Painter::addShape(Shape *shape) {
	printf("adding a shape to shapes\n");
	this->shapes.addShape(shape);
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
	pugi::xml_node listOfShapes = doc.child("shapes");
	shapes.parseXML(&listOfShapes);
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
		sketch.show();
		commandWin.show();
	}
	else{
		sketch.hide();
		commandWin.hide();
	}
}
void Painter::launchSimulatorWindow(){
	//this->shapes.drawAll(this->simWin);
	printf("drawing a shape of some kind");
	this->simWin->show();
}
void Painter::launchCommandWindow(){
	commandWin.show();
}