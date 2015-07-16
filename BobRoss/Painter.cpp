#include <Painter.h>

Painter::Painter() {
}
Painter::Painter(std::string name) {
}
Painter::Painter(Shapes shapes) {
}
Painter::Painter(std::string name, Shapes shapes) {
}
void Painter::addShape(Shape shape) {
}
void Painter::setDimensions(int width, int height) {
}
void Painter::save() {
}
void Painter::load(std::string projectName, std::string projectLocation) {
}
void Painter::setName(std::string ProjectName){
}
void Painter::setLocation(std::string ProjectLocation){
}
void Painter::showGUI(bool toggle){
}
void Painter::launchSimulatorWindow(){
}
void Painter::launchCommandWindow(){
}