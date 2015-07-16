#include <Painter.h>

Painter::Painter() {
	ProjectName = "Temp";
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
	simWin = new DrawWindow(width,height,ProjectName);
}
Painter::Painter(std::string name) {
	ProjectName = name;
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
	simWin = new DrawWindow(width, height, ProjectName);
}
Painter::Painter(Shapes shapes) {
	this->shapes = shapes;
	ProjectName = "Temp";
	ProjectLocation = "";
	width = 1000;
	height = 800;

	sketch = new Sketchpad(width, height);
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
}
void Painter::save() {
	std::cout << "saving" << std::endl;
}
void Painter::load(std::string projectName, std::string projectLocation) {
	std::cout << "preparing to load from " << projectName << "/" << projectLocation << ".xml" << std::endl;
}
void Painter::setName(std::string ProjectName){
	this->ProjectName = ProjectName;
}
void Painter::setLocation(std::string ProjectLocation){
	this->ProjectLocation = ProjectLocation;
}
void Painter::showGUI(bool toggle){
	if (toggle){
		sketch->show();
		commandWin.show();
	}
	else{
		sketch->hide();
		commandWin.hide();
	}
}
void Painter::launchSimulatorWindow(){
	this->simWin->show();
	this->shapes.drawAll(this->simWin);
	printf("drawing a shape of some kind");
}
void Painter::launchCommandWindow(){
	commandWin.show();
}