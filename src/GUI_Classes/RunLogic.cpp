#include "RunLogic.h"
#include <windows.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief constructor
 * @param width
 * @param height
 * @param shapes
 */
RunLogic::RunLogic(int width, int height, Shapes *shapes, CytonRunner *Ava, ABBRunner *Chappie) {
	COMMAND_DELAY = 10; //ms
	this->width = width;
	this->height = height;
	this->shapes = shapes;
	this->Ava = Ava;
	this->chappie = Chappie;
	this->simWin = new DrawWindow(width, height, "Simulation Window");
	clearClicked();
	simWin->hideWindow();
	mode = "Simulate Magic Marker";
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
	if (stepTaken == BACKWARD) {
		currentShapeIndex++;
	}
	if (currentShapeIndex >= shapes->length()) {
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


	if (currentShapeIndex == 0 && stopIndex == 0) {
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
void RunLogic::reset() {
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
void RunLogic::resetStepTaken() {
	stepTaken = NEITHER;
}

//hides the simwin
void RunLogic::hideSimWin() {
	this->simWin->hideWindow();
}
//clears the simwin to white
void RunLogic::clearSimWin() {
	this->simWin->clearWindow(255, 255, 255);
}

//makes an entirely new simwin
void RunLogic::resetSimWin(int width, int height, std::string string) {
	this->simWin = new DrawWindow(width, height, string);
}
//shows the simwin
void RunLogic::showSimWin() {
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

	// shouldn't be needed as defined in the menu options
	//Ava->curBrush = new Brush(30, 20, "ellipse");

	// this should be a user-defined toggle (eventually)
	this->Ava->curBrush->setDrawMode("paint");

	if (mode == "Simulate Magic Marker") {
		this->Ava->curBrush->setDrawMode("normal");
		auto d1 = std::async(&RunLogic::paintThread, this, simWin);
	}
	else if (mode == "Simulate Real Brush") {
		auto d1 = std::async(&RunLogic::paintThread, this, simWin);
	}
	else if (mode == "Non-touch robot motion") {
		printf("mode not yet developed\n");
	}
	else if (mode == "Paint w/o feedback") {
		if (!Ava->connected && !chappie->connected) { printf("Please connect the robot before continuing\n"); return; }
		Ava->curBrush = new Brush(2, 2, "ellipse");
		auto d1 = std::async(&RunLogic::paintThread, this, simWin);
	}
	else if (mode == "Paint w/ feedback") {
		printf("mode not yet implemented\n");
	}
}

//thread to handle actual drawing.
void RunLogic::paintThread(DrawWindow *W) {
	while (running && currentShapeIndex < stopIndex) {
		Shape *s = this->shapes->at(currentShapeIndex); //Updating shape

		if (s->isBreakPoint) { //Handling breakpoint
			running = false;
			return;
		}

		this->setAvaPenColor(s);

		emit updateCommandList(currentShapeIndex, "runningBot");
		if (s->fill) { paintFill(W, s); } //paint fill
		else { drawPolyLine(s->toPolyline()->points, W); } //paint polyline
		emit updateCommandList(currentShapeIndex, "finishedSim");

		currentShapeIndex++;
	}
	if (currentShapeIndex == stopIndex) currentShapeIndex--;
	if (chappie->connected) {
		chappie->end();
	}
	running = false;
}

//paint a fill object
void RunLogic::paintFill(DrawWindow *W, Shape *s) {
	RegionToPaths RTP = RegionToPaths(width, height, 30);
	PixelRegion *p = s->toPixelRegion();
	std::vector<cv::Point> pts = p->getPoints();

	RTP.setAllPixels("overpaintable");

	// don't let brush/pen overpaint non-white pixels
	for (int i = 0; i < W->width; i++) {
		for (int j = 0; j < W->height; j++) {
			if (!W->testPixel(i, j, 255, 255, 255)) {
				RTP.addUntouchablePixel(i, j);
			}
		}
	}

	for (size_t i = 0; i < pts.size(); i++) { RTP.addDesiredPixel(pts.at(i).x, pts.at(i).y); }
	// put a halo of N pixels around the desired region where allowed to paintover (could be done in define path too)
	RTP.expandOverpaintableAroundDesired(3);

	if (chappie->connected) {
		Ava->curBrush->setWidth(10);
		Ava->curBrush->setHeight(10);
	}
	RTP.defineBrush(Ava->curBrush);

	RTP.definePaths();

	/*	DrawWindow DebugW(width, height, "debug window");
	RTP.definePaths(&DebugW);
	DebugW.moveWindow(400, 200);
	DebugW.show(); */

	// note: getBrushStrokes returns vector of *contiguous* pixel points
	std::vector<std::vector<cv::Point>> brush_strokes = RTP.getBrushStrokes();

	// ABC: TBD toggle between "pen" and "paint"
	if (0) { Ava->curBrush->setDrawMode("paint"); }

	for (size_t i = 0; i < brush_strokes.size(); i++) { //running through vector of polylines
		this->doStroke(brush_strokes.at(i), W, false);
	}
}


// given contiguous pts, draw them in the simulator and optionally using robot
void RunLogic::doStroke(std::vector<cv::Point> pts, DrawWindow *W, bool ignoreSmall) {
	Ava->curBrush->loadPaintPixels(); // should be in "getPaint()";
	Ava->curBrush->drawContiguousPoints(W, &pts);
	if (chappie->connected && running) { 
		for (int i = 0; i < pts.size(); i++) { //running through points in one stroke
			i = straighten(pts, i);
			//printf("%d/%d\n", i,pts.size()-1);
			W->show();
			chappie->update();
			if (!chappie->sendCoord(pts.at(i).x, pts.at(i).y)) { break; }
		}
	}

	W->show();
	chappie->next();
}


void RunLogic::drawPolyLine(std::vector<cv::Point> pts, DrawWindow *W) {
	int prevX = pts.at(0).x; int prevY = pts.at(0).y; //initializing loop vars

	if (chappie->connected) {
		if (!chappie->sendCoord(prevX, prevY)) {
			return;
		}
	}

	for (size_t i = 1; i < pts.size(); i++) { //running through points in one stroke
		if (!running) { return; }

		if (Ava->connected) { //connected, polyline
			Ava->stroke(cv::Point(prevX, prevY), pts.at(i));
			Ava->curBrush->drawLine(W, prevX, prevY, pts.at(i).x, pts.at(i).y);
		}
		else { Ava->curBrush->drawLine(W, prevX, prevY, pts.at(i).x, pts.at(i).y); }
		if (chappie->connected) {
			if (!chappie->sendCoord(pts.at(i).x, pts.at(i).y)) {
				return;
			}
		}
		//update loop vars
		prevX = pts.at(i).x;
		prevY = pts.at(i).y;

		W->show();
	}
	if (chappie->connected) {
		if (!chappie->next()) {
			return;
		}
	}
	Sleep(30);
	Ava->strokeInProgress = false;
}

void RunLogic::setAvaPenColor(Shape *s) {
	cv::Scalar temp = s->getPenColor();
	if (chappie->connected) {
		chappie->decidePaint(s->pos);
	}
	if (Ava->paint.size() >= 2) {
		Ava->decidePaint(temp[0], temp[1], temp[2]);
	}
	else {
		Ava->curBrush->setColor(s->getPenColor());
	}
}

int RunLogic::straighten(std::vector<cv::Point> pts, int index) {
	if (index == 0) { return index; }					 //of course you draw the first point, you twit
	if (endCheck(pts, index)) { return pts.size() - 1; } //of course you draw the last point, you twit
	int done = 0; int originalIndex = index;

	double a1 = angleDiff(pts.at(index), pts.at(index + 1));
	double a2;
	while (!done) {
		index++;
		if (endCheck(pts, index)) { return pts.size() - 1; } //of course you draw the last point, you twit
		if (tooFar(pts.at(index), pts.at(originalIndex))) { return index; }
		if (tooClose(pts.at(index), pts.at(originalIndex))) { continue; }
		a2 = angleDiff(pts.at(index), pts.at(index + 1));
		done = abs(a1 - a2) > 0.122173; // 7 degrees
	}
	return index;
}

bool RunLogic::endCheck(std::vector<cv::Point> pts, int index) {
	return index >= pts.size() - 1;
}

double RunLogic::angleDiff(cv::Point p1, cv::Point p2) {
	return atan2((p1.y - p2.y), (p1.x - p2.x));
}

bool RunLogic::tooClose(cv::Point p1, cv::Point p2) {
	return 10 > sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

bool RunLogic::tooFar(cv::Point p1, cv::Point p2) {
	return 50 < sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}