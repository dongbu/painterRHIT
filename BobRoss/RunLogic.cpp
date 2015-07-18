#include "RunLogic.h"

RunLogic::RunLogic(int width, int height, Shapes *shapes) {
	this->width = width;
	this->height = height;
	this->shapes = shapes;
	this->simWin = new DrawWindow(width, height, "simulation window");
}

void RunLogic::stopClicked() {
	printf("TODO: reset position and toggles\n");
	this->shapes->setRunning(false);
	this->simWin->clearWindow(255, 255, 255);
}
void RunLogic::pauseClicked() {
	this->shapes->setRunning(false);
}
void RunLogic::forwardClicked() {
	printf("TODO: implement forwardClicked\n");
}
void RunLogic::backwardClicked() {
	printf("TODO: implement backwardClicked\n");
}
void RunLogic::runClicked() {
	printf("TODO: keep track of position\n");
	printf("TODO: change colors\n");
	printf("TODO: deal with breakpoints and toggles\n");
	this->simWin->showWindow();
	this->shapes->drawAll(this->simWin);
}

void RunLogic::runFrom(int index) {
	printf("TODO: implement runFrom\n");
}
void RunLogic::runOnly(int index) {
	printf("TODO: implement runOnly\n");
}
void RunLogic::setBreakPoint(int index) {
	printf("TODO: implement setBreakPoint\n");
}