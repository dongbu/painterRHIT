#include "RunLogic.h"

int COMMAND_DELAY = 1000; //(ms)

RunLogic::RunLogic(int width, int height, Shapes *shapes) {
	this->width = width;
	this->height = height;
	this->shapes = shapes;
	this->simWin = new DrawWindow(width, height, "simulation window");
	this->simWin->hideWindow();
	stopClicked();
}

void RunLogic::shapesChanged() { stopIndex = shapes->length(); }

void RunLogic::stopClicked() {
	this->simWin->showWindow();
	running = false;
	simWin->clearWindow(255, 255, 255); //white
	currentShapeIndex = 0;
	stopIndex = shapes->length();
}
void RunLogic::pauseClicked() { running = false; }
	
void RunLogic::forwardClicked() {
	this->simWin->showWindow();
	if (currentShapeIndex >= shapes->length()) return;
	running = false;
	runOnly(currentShapeIndex);
	currentShapeIndex++;
}
void RunLogic::backwardClicked() {
	this->simWin->showWindow();
	if (currentShapeIndex <= 0) return;
	currentShapeIndex--; //otherwise, -- the index
	simWin->clearWindow(255, 255, 255); //white
	running = false;
	for (int i = 0; i < currentShapeIndex; i++) runOnly(i);
}
void RunLogic::runClicked() {
	this->simWin->showWindow();
	if (running) return; //don't start multiple window threads (that's bad...)
	running = true;
	auto d1 = std::async(&RunLogic::drawingThread, this, simWin);
}

void RunLogic::runFrom(int index) {
	this->simWin->showWindow();
	currentShapeIndex = index;
	stopIndex = shapes->length();
	runClicked();
}
void RunLogic::runOnly(int index) {
	this->simWin->showWindow();
	shapes->at(index)->draw(simWin);
	simWin->show();
}
void RunLogic::setBreakPoint(int index) {
	printf("TODO: implement setBreakPoint\n");
}

void RunLogic::drawingThread(DrawWindow *W) {
	while (running && currentShapeIndex < stopIndex) {
		shapes->at(currentShapeIndex)->draw(W);
		W->show();
		currentShapeIndex++;
		_sleep(COMMAND_DELAY);
	}
	running = false;
}