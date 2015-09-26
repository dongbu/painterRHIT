#include "RunLogic.h"
#include <windows.h>

/**
 * @brief constructor
 * @param width
 * @param height
 * @param shapes
 */
RunLogic::RunLogic(int width, int height, Shapes *shapes, CytonRunner *Ava) {
	COMMAND_DELAY = 10; //ms
	this->width = width;
	this->height = height;
	this->shapes = shapes;
	this->Ava = Ava;
	this->simWin = new DrawWindow(width, height, "Simulation Window");
	clearClicked();
	simWin->hideWindow();
	mode = "Simulate Idealized Brush";
}

/**
 * @brief updates the stop index.
 */
void RunLogic::shapesChanged() { stopIndex = shapes->length(); }

/**
 * @brief stops and clears simulation.
 */
void RunLogic::clearClicked() {
	stepTaken = NEITHER;
	running = false;
	simWin->clearWindow(255, 255, 255); //white
	simWin->show();
	currentShapeIndex = 0;
	stopIndex = shapes->length();
	for (int i = 0; i < shapes->length(); i++) {
		emit updateCommandList(i, "clear");
	}
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
	if (stepTaken == BACKWARD){
		currentShapeIndex++;
	}
	if (currentShapeIndex >= shapes->length()){
		stepTaken = NEITHER;
		return;
	}

	running = false;
	runOnly(currentShapeIndex);
	stepTaken = FORWARD;
}
/**
 * @brief steps backwards.
 */
void RunLogic::backwardClicked() {


	if (currentShapeIndex == 0 && stopIndex == 0){
		stepTaken = NEITHER;
		return;
	}

	this->simWin->showWindow();
	running = false;
	emit updateCommandList(currentShapeIndex, "clear");
	simWin->clearWindow(255, 255, 255); //white
	if (currentShapeIndex <= 0) {
		stepTaken = NEITHER;
		simWin->show();
		return;
	}
	int temp = currentShapeIndex;
	for (int i = 0; i < temp; i++) {
		runOnly(i);
		currentShapeIndex--;
	}
	stepTaken = BACKWARD;
}
/**
 * @brief runs simulation.
 */
void RunLogic::runClicked() {
	this->simWin->showWindow();
	if (running) return; //don't start multiple window threads (that's bad...)
	running = true;
	stepTaken = NEITHER;
	if (mode == "Simulate Idealized Brush") {
		auto d1 = std::async(&RunLogic::simulateDelayedBrushThread, this, simWin);
	}
	else if (mode == "Simulate Real Brush") {
		printf("mode not yet implemented\n");
	}
	else if (mode == "Non-touch robot motion") {
		printf("mode not yet developed\n");
	}
	else if (mode == "Paint w/o feedback") {
		if (!Ava->connected) { printf("Please connect the robot before continuing\n"); return; }
		auto d1 = std::async(&RunLogic::paintWOFeedbackThread, this, simWin);
	}
	else if (mode == "Paint w/ feedback") {
		printf("mode not yet implemented\n");
	}
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
	stepTaken = NEITHER;
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

	emit(updateCommandList(index, "finishedSim"));
	simWin->show();
}

//clear away everything and reset variables.
void RunLogic::reset(){
	stepTaken = NEITHER;
	shapes->clear();
	clearClicked();
	simWin = new DrawWindow(width, height, "Simulation Window");
	//simWin->hideWindow();
}

void RunLogic::updateMode(QString mode, int delay) {
	this->mode = mode;
	this->COMMAND_DELAY = delay;
}

//thread to handle actual drawing.
void RunLogic::simulateDelayedBrushThread(DrawWindow *W){
	printf("starting delayed brush thread\n");
	while (running && currentShapeIndex < stopIndex) {
		//Updating index
		Shape *s = this->shapes->at(currentShapeIndex);
		//handling breakpoint
		if (s->isBreakPoint) {
			running = false;
			return;
		}
		if (s->fill) { //painting filled region
			paintFill(W, s, true);
		}
		else { //painting polyline object
			paintPolyline(W, s, true);
		}
		emit updateCommandList(currentShapeIndex, "finishedSim");

		currentShapeIndex++;
	}
	if (currentShapeIndex == stopIndex) currentShapeIndex--;
	running = false;
}

//thread to handle actual drawing.
void RunLogic::paintWOFeedbackThread(DrawWindow *W){
	printf("starting painting w/o feedback 'thread'\n");
	while (running && currentShapeIndex < stopIndex) {
		//Updating index
		Shape *s = this->shapes->at(currentShapeIndex);
		//handling breakpoint
		if (s->isBreakPoint) {
			running = false;
			return;
		}

		if (s->fill) { //painting filled region
			paintFill(W, s, false);
		}
		else { //painting polyline object
			paintPolyline(W, s, false);
		}
		emit updateCommandList(currentShapeIndex, "finishedBot");

		currentShapeIndex++;
	}
	if (currentShapeIndex == stopIndex) currentShapeIndex--;
	running = false;
	Ava->changePaint(0);
	Ava->goToJointHome(1);
}

//hides the simwin
void RunLogic::hideSimWin(){
	this->simWin->hideWindow();
}
//clears the simwin to white
void RunLogic::clearSimWin(){
	this->simWin->clearWindow(255, 255, 255);
}

//makes an entirely new simwin
void RunLogic::resetSimWin(int width, int height, std::string string){
	this->simWin = new DrawWindow(width, height, string);
}
//shows the simwin
void RunLogic::showSimWin(){
	this->simWin->showWindow();
	this->simWin->show();
}

//paint a polyline object
void RunLogic::paintPolyline(DrawWindow *W, Shape *s, bool simulated){
	emit updateCommandList(currentShapeIndex, "runningBot");
	std::vector<cv::Point> pts = s->toPolyline()->points;

	if (!simulated){
		if (Ava->connected && Ava->paint.size() >= 2){
			cv::Scalar temp = s->getPenColor();
			int r = temp[0];
			int g = temp[1];
			int b = temp[2];
			Ava->decidePaint(r, g, b);
		}
	}

	int prevX = pts.at(0).x;
	int prevY = pts.at(0).y;
	for (size_t i = 1; i < pts.size(); i++) { //running through points in one stroke
		if (!running) { return; }

		if (Ava->connected || !simulated) { //connected, polyline
			Ava->stroke(cv::Point(prevX, prevY), pts.at(i));
			Ava->curBrush->drawLine(W, prevX, prevY, pts.at(i).x, pts.at(i).y);

		}
		else { //not connected, polyline
			Brush curBrush = Brush(15, 10, "ellipse");
			curBrush.setColor(s->getPenColor());
			curBrush.drawLine(W, prevX, prevY, pts.at(i).x, pts.at(i).y);
		}
		prevX = pts.at(i).x;
		prevY = pts.at(i).y;

		W->show();
	}
	Ava->strokeInProgress = false;
}

//paint a fill object
void RunLogic::paintFill(DrawWindow *W, Shape *s, bool simulated){
	emit updateCommandList(currentShapeIndex, "runningBot");
	Brush *curBrush;
	RegionToPaths RTP = RegionToPaths(width, height, 30);
	PixelRegion *p = s->toPixelRegion();
	std::vector<cv::Point> pts = p->getPoints();

	if (!simulated){
		if (Ava->paint.size() >= 2){
			cv::Scalar temp = s->getPenColor();
			int r = temp[0];
			int g = temp[1];
			int b = temp[2];
			Ava->decidePaint(r, g, b);
		}
	}

	for (int j = 0; j < W->grid.size().height; j++) {
		for (int k = 0; k < W->grid.size().width; k++) {
			RTP.addOverpaintablePixel(k, j);
		}
	}
	for (size_t i = 0; i < pts.size(); i++){ RTP.addDesiredPixel(pts.at(i).x, pts.at(i).y); }

	curBrush = Ava->curBrush;
	RTP.defineBrush(Ava->curBrush);

	RTP.definePaths();
	std::vector<std::vector<cv::Point>> pathVec = RTP.getBrushStrokes();
	for (size_t i = 0; i < pathVec.size(); i++){ //running through vector of strokes
		int prevX = pathVec.at(i).at(0).x;
		int prevY = pathVec.at(i).at(0).y;
		for (size_t j = 1; j < pathVec.at(i).size(); j++) { //running through points in one stroke
			if (!running) { return; }
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (!simulated){
				Ava->stroke(cv::Point(prevX, prevY), pathVec.at(i).at(j));
			}
			curBrush->drawLine(W, prevX, prevY, pathVec.at(i).at(j).x, pathVec.at(i).at(j).y);
			prevX = pathVec.at(i).at(j).x;
			prevY = pathVec.at(i).at(j).y;

			W->show();
		}
		Ava->strokeInProgress = false;
	}
}

//resets stepTaken to NEITHER
void RunLogic::resetStepTaken(){
	stepTaken = NEITHER;
}