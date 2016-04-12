#include "RunLogic.h"
#include <windows.h>
#include <stdlib.h>

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
		Ava->curBrush = new Brush(4, 4, "ellipse");
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
		printf("doing stroke %lu/%lu (%lu points)\n", i, brush_strokes.size(), brush_strokes.at(i).size());
		this->doStroke(brush_strokes.at(i), W, false);
	}
}


// given contiguous pts, draw them in the simulator and optionally using robot
void RunLogic::doStroke(std::vector<cv::Point> pts, DrawWindow *W, bool ignoreSmall) {
	int nloops = 0;
	int done = 0;
	while (!done) {
		nloops++;

		// attempt to draw stroke in simulator window
		// TBD: somewhere need to set toggle if simulating real paint
		Ava->curBrush->loadPaintPixels(); // should be in "getPaint()";
		Ava->curBrush->drawContiguousPoints(W, &pts);

		// if want to use the robot, do so
		if ((Ava->connected || chappie->connected) && running) { //if(0 && Ava->connected && running){
			int prevX = pts.at(0).x; int prevY = pts.at(0).y; //initializing loop vars
			if (chappie->connected) {
				if (ignoreSmall) { ////////GUNNAR FIX THIS!!!!!!!!!!!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////
					int tempX = abs(pts.at(0).x - pts.at(pts.size() - 1).x);
					int tempY = abs(pts.at(0).y - pts.at(pts.size() - 1).y);
					if ((pts.size() <= 2 && (tempX <= 5 && tempY <= 5)) || (tempX <= 3 && tempY <= 3)) {
						printf("ignoring stroke\n");
						return;
					}
				}
				chappie->sendCoord(prevX, prevY);
			}
			int maxPixelstoTry = 10000;  // some limit as expected to run out of paint (unless perhaps using a pen)
			int c = 0;
			for (int i = 1; i < pts.size(); i++) { //running through points in one stroke
				c++;
				if (c < maxPixelstoTry) {
					if (Ava->connected) {
						Ava->stroke(cv::Point(prevX, prevY), pts.at(i));
					}
					else if (chappie->connected) {////////////////////////////////////////////////////////////////////////////////////////////////////////
						done = 1;
						if (i == pts.size() - 1 || i%10 == 0) {
						if (!chappie->sendCoord(pts.at(i).x, pts.at(i).y)) {
							break;
						}
						}
					}
					//update loop vars
					prevX = pts.at(i).x;
					prevY = pts.at(i).y;
				}
			}
		}

		// check results of work to see if done
		std::vector<cv::Point> errors; // potentially non-contiguous points that still need to be painted
		double score; // what % of pixels are of the desired paint color
		int webcam_feedback = 0; // eventually a toggle
		if (webcam_feedback) {
			//cv::mat WEBCAM_IMAGE = Ava->getWebcamImageOfPainting();
			//score = brush.scorePaintPoints(&WEBCAM_IMAGE, &stroke, &errors, .1, .1);
		}
		else {
			//score = Ava->curBrush->scorePaintPoints(W, &pts, &errors, .1, .1); //this line is causing crashing, atm.
			score = 0;
		}

		if (score > 0.0 && score < 0.99) { //not: if score=0, then rerun the entire stroke
			// redefine pts to be all the points starting from the first error
			cv::Point first_error = errors.at(0);
			std::vector<cv::Point> next_loop_pts;
			int found = 0;
			for (size_t i = 1; i < pts.size(); i++) { //running through points in one stroke
				if (pts.at(i).x == first_error.x && pts.at(i).y == first_error.y) { found = 1; }
				if (found) {
					next_loop_pts.push_back(pts.at(i));
				}
			}
			printf(" reloop %i (score:%.2f) - %lu -> %lu points\n", nloops, 100 * score, pts.size(), next_loop_pts.size());
			pts = next_loop_pts;
			Sleep(100);
		}

		//Sleep(30);

		// loop limit just so it doesn't loop forever in case brush fails to fully cover stroke
		if (score > 0.99 || nloops > 12) { done = 1; }

		W->show();
	}

	if (chappie->connected) {
		if (!chappie->next()) {
			return;
		}
	}
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
		chappie->decidePaint(temp[0], temp[1], temp[2]);
	}
	if (Ava->paint.size() >= 2) {
		Ava->decidePaint(temp[0], temp[1], temp[2]);
	}
	else {
		Ava->curBrush->setColor(s->getPenColor());
	}
}