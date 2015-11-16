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

//resets stepTaken to NEITHER
void RunLogic::resetStepTaken(){
	stepTaken = NEITHER;
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

//############################################ GUI Logic Above HERE ###################################################################//
//############################################ Simulation Logic Below Here############################################################//
/**
* @brief runs simulation.
*/
void RunLogic::runClicked() {
	this->simWin->showWindow();
	if (running) return; //don't start multiple window threads (that's bad...)
	running = true;
	stepTaken = NEITHER;
	if (mode == "Simulate Idealized Brush") {
		auto d1 = std::async(&RunLogic::paintThread, this, simWin, true);
	}
	else if (mode == "Simulate Real Brush") {
		printf("mode not yet implemented\n");
	}
	else if (mode == "Non-touch robot motion") {
		printf("mode not yet developed\n");
	}
	else if (mode == "Paint w/o feedback") {
		if (!Ava->connected) { printf("Please connect the robot before continuing\n"); return; }
		auto d1 = std::async(&RunLogic::paintThread, this, simWin, false);
	}
	else if (mode == "Paint w/ feedback") {
		printf("mode not yet implemented\n");
	}
}

//thread to handle actual drawing.
void RunLogic::paintThread(DrawWindow *W, bool simulated){
	while (running && currentShapeIndex < stopIndex) {
		Shape *s = this->shapes->at(currentShapeIndex); //Updating shape

		if (s->isBreakPoint) { //Handling breakpoint
			running = false;
			return;
		}

		this->setAvaPenColor(simulated, s);

		emit updateCommandList(currentShapeIndex, "runningBot");
		if (s->fill) { paintFill(W, s, simulated); } //paint fill
		else { drawPolyLine(s->toPolyline()->points,simulated,W); } //paint polyline
		emit updateCommandList(currentShapeIndex, "finishedSim");

		currentShapeIndex++;
	}
	if (currentShapeIndex == stopIndex) currentShapeIndex--;
	running = false;
}

//paint a fill object
void RunLogic::paintFill(DrawWindow *W, Shape *s, bool simulated){
	RegionToPaths RTP = RegionToPaths(width, height, 30);
	PixelRegion *p = s->toPixelRegion();
	std::vector<cv::Point> pts = p->getPoints();

	for (int j = 0; j < W->grid.size().height; j++) {
		for (int k = 0; k < W->grid.size().width; k++) {
			RTP.addOverpaintablePixel(k, j);
		}
	}
	for (size_t i = 0; i < pts.size(); i++){ RTP.addDesiredPixel(pts.at(i).x, pts.at(i).y); }

	RTP.defineBrush(Ava->curBrush);
	RTP.definePaths();
	std::vector<std::vector<cv::Point>> pathVec = RTP.getBrushStrokes();

	for (size_t i = 0; i < pathVec.size(); i++){ //running through vector of polylines
		this->drawPolyLine(pathVec.at(i), simulated, W);
	}
}

void RunLogic::drawPolyLine(std::vector<cv::Point> pts, bool simulated, DrawWindow *W) {
	int prevX = pts.at(0).x; int prevY = pts.at(0).y; //initializing loop vars
	for (size_t i = 1; i < pts.size(); i++) { //running through points in one stroke
		if (!running) { return; }

		if (Ava->connected && !simulated) { //connected, polyline
			Ava->stroke(cv::Point(prevX, prevY), pts.at(i));
			Ava->curBrush->drawLine(W, prevX, prevY, pts.at(i).x, pts.at(i).y);
		}
		else { Ava->curBrush->drawLine(W, prevX, prevY, pts.at(i).x, pts.at(i).y); }

		//update loop vars
		prevX = pts.at(i).x;
		prevY = pts.at(i).y;

		W->show();
	}
	Ava->strokeInProgress = false;
}

void RunLogic::setAvaPenColor(bool simulated, Shape *s) {
	if (!simulated && Ava->paint.size() >= 2){
		Ava->curBrush = new Brush(30, 20, "ellipse");
		cv::Scalar temp = s->getPenColor();
		Ava->decidePaint(temp[0], temp[1], temp[2]);
	}
	else {
		Ava->curBrush = new Brush(10, 10, "ellipse");
		Ava->curBrush->setColor(s->getPenColor());
	}
}