#include "RunLogic.h"
#include <windows.h>

int COMMAND_DELAY = 10; //(ms)

/**
 * @brief constructor
 * @param width
 * @param height
 * @param shapes
 */
RunLogic::RunLogic(int width, int height, Shapes *shapes, CytonRunner *Ava) {
	this->width = width;
	this->height = height;
	this->shapes = shapes;
	this->Ava = Ava;
	this->simWin = new DrawWindow(width, height, "Simulation Window");
	stopClicked();
	simWin->hideWindow();
	connect(Ava, SIGNAL(finishedShape()), this, SLOT(runClicked()));
}

/**
 * @brief updates the stop index.
 */
void RunLogic::shapesChanged() { stopIndex = shapes->length(); }

/**
 * @brief stops and clears simulation.
 */
void RunLogic::stopClicked() {
	stepTaken = 2;
	running = false;
	simWin->clearWindow(255, 255, 255); //white
	simWin->show();
	currentShapeIndex = 0;
	stopIndex = shapes->length();
	emit clearRunColors();
}
/**
 * @brief pauses simulation.
 */
void RunLogic::pauseClicked() { running = false; }

/**
 * @brief steps forward.
 */
void RunLogic::forwardClicked() {

	this->simWin->showWindow();
	if (stepTaken == 0){
		currentShapeIndex++;
	}
	if (currentShapeIndex >= shapes->length()){
		stepTaken = 2;
		return;
	}

	running = false;
	runOnly(currentShapeIndex);
	stepTaken = 1;
}
/**
 * @brief steps backwards.
 */
void RunLogic::backwardClicked() {


	if (currentShapeIndex == 0 && stopIndex == 0){
		stepTaken = 2;
		return;
	}

	this->simWin->showWindow();
	running = false;
	emit setRunColor(currentShapeIndex, "white");
	simWin->clearWindow(255, 255, 255); //white
	if (currentShapeIndex <= 0) {
		stepTaken = 2;
		simWin->show();
		return;
	}
	printf("going backward.  ShapeIndex is: %i  shapes->length() is: %i\n", currentShapeIndex, shapes->length());
	int temp = currentShapeIndex;
	for (int i = 0; i < temp; i++) {
		runOnly(i);
		currentShapeIndex--;
	}
	stepTaken = 0;
}
/**
 * @brief runs simulation.
 */
void RunLogic::runClicked() {
	this->simWin->showWindow();
	if (running) return; //don't start multiple window threads (that's bad...)
	running = true;
	if (!Ava->connected) { auto d1 = std::async(&RunLogic::SimulationThread, this, simWin); }
	else { auto d1 = std::async(&RunLogic::RobotThread, this, simWin); }

	stepTaken = 2;
}

/**
 * @brief runs simulation from specified index.
 * @param index
 */
void RunLogic::runFrom(int index) {
	this->simWin->showWindow();
	currentShapeIndex = index;
	stopIndex = shapes->length();
	runClicked();
	stepTaken = 2;
}
/**
 * @brief runs only the specified command.
 * @param index
 */
void RunLogic::runOnly(int index) {
	this->simWin->showWindow();
	shapes->at(index)->draw(simWin);
	currentShapeIndex = index + 1;
	if (currentShapeIndex == stopIndex) currentShapeIndex--;

	emit(setRunColor(index, "green"));
	simWin->show();
}
/**
 * @brief toggles breakpoint on specified index.
 * @param index
 */
void RunLogic::toggleBreakPoint(int index) {
	if (!shapes->at(index)->isBreakPoint) {
		shapes->at(index)->toggleBreakPoint(true);
		emit setRunColor(index, "red");
	} else {
		shapes->at(index)->toggleBreakPoint(false);
		emit setRunColor(index, "white");
	}
}
/**
 * @brief thread for actual drawing.
 * @param W
 */
void RunLogic::SimulationThread(DrawWindow *W) {
	while (running && currentShapeIndex < stopIndex) {
		if (shapes->at(currentShapeIndex)->isBreakPoint){
			toggleBreakPoint(currentShapeIndex);
			running = false;
			return;
		}
		shapes->at(currentShapeIndex)->draw(W);
		emit setRunColor(currentShapeIndex, "green");
		W->show();
		currentShapeIndex++;
		Sleep(COMMAND_DELAY);
	}
	if (currentShapeIndex == stopIndex) currentShapeIndex--;
	running = false;
}


void RunLogic::RobotThread(DrawWindow *W){
	while (running && currentShapeIndex < stopIndex) {
		printf("current shape, stop index (%i,%i)\n", currentShapeIndex, stopIndex);
		//Updating index
		Shape *s = this->shapes->at(currentShapeIndex);

		//handling breakpoint
		if (s->isBreakPoint) {
			toggleBreakPoint(currentShapeIndex);
			running = false;
			return;
		}

		//drawing shape on robot && simulator
		if (s->fill) { //painting filled region
			RegionToPaths RTP = RegionToPaths(width, height, 30);
			PixelRegion *p = s->toPixelRegion();
			std::vector<cv::Point> pts = p->getPoints();
			for (size_t i = 0; i < pts.size(); i++){ RTP.addDesiredPixel(pts.at(i).x, pts.at(i).y); }
			RTP.defineBrush(Ava->curBrush);
			RTP.definePaths();
			std::vector<std::vector<cv::Point>> pathVec = RTP.getBrushStrokes();
			for (size_t i = 0; i < pathVec.size(); i++){ //running through vector of strokes
				if (!running) { return; }
				int prevX = pathVec.at(i).at(0).x;
				int prevY = pathVec.at(i).at(0).y;
				for (size_t j = 1; j < pathVec.at(i).size(); j++) { //running through points in one stroke
					emit setRunColor(currentShapeIndex, "yellow");

					Ava->stroke(cv::Point(prevX, prevY), pathVec.at(i).at(j));
					Ava->curBrush->drawLine(this->simWin, prevX, prevY, pathVec.at(i).at(j).x, pathVec.at(i).at(j).y);
					prevX = pathVec.at(i).at(j).x;
					prevY = pathVec.at(i).at(j).y;

					W->show();
				}
				Ava->strokeInProgress = false;
			}
		} else { //painting polyline object
			std::vector<cv::Point> pts = s->toPolyline()->points;

			int prevX = pts.at(0).x;
			int prevY = pts.at(0).y;
			for (size_t i = 1; i < pts.size(); i++) { //running through points in one stroke
				if (!running) { return; }
				emit setRunColor(currentShapeIndex, "yellow");

				Ava->stroke(cv::Point(prevX, prevY), pts.at(i));
				Ava->curBrush->drawLine(this->simWin, prevX, prevY, pts.at(i).x, pts.at(i).y);
				prevX = pts.at(i).x;
				prevY = pts.at(i).y;

				W->show();
			}
			Ava->strokeInProgress = false;
		}
		emit setRunColor(currentShapeIndex, "green");
		currentShapeIndex++;
	}
}

void RunLogic::reset(){
	stepTaken = 2;
	shapes->clear();
	stopClicked();
	simWin = new DrawWindow(width, height, "Simulation Window");
	//simWin->hideWindow();
}