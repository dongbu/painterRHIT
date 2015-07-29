#include "RunLogic.h"

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
    this->simWin = new DrawWindow(width, height, "simulation window");
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
    if (currentShapeIndex >= shapes->length()) return;
    running = false;
    runOnly(currentShapeIndex);
}
/**
 * @brief steps backwards.
 */
void RunLogic::backwardClicked() {
    this->simWin->showWindow();
	running = false;
    emit setRunColor(currentShapeIndex, false);
	simWin->clearWindow(255, 255, 255); //white
	if (currentShapeIndex <= 0) {
		simWin->show();
		return;
	}

	int temp = currentShapeIndex;
	for (int i = 0; i < temp; i++) {
		runOnly(i);
		currentShapeIndex--;
	}
}
/**
 * @brief runs simulation.
 */
void RunLogic::runClicked() {
    this->simWin->showWindow();
    if (running) return; //don't start multiple window threads (that's bad...)
    running = true;
    auto d1 = std::async(&RunLogic::drawingThread, this, simWin);
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
    emit(setRunColor(index, true));
    simWin->show();
}
/**
 * @brief toggles breakpoint on specified index.
 * @param index
 */
void RunLogic::toggleBreakPoint(int index) {
    if (shapes->at(index)->isBreakPoint){
        shapes->at(index)->toggleBreakPoint(false);
        emit setBreakPointColor(index, false);
    }
    else{
        shapes->at(index)->toggleBreakPoint(true);
        emit setBreakPointColor(index, true);
    }

}
/**
 * @brief thread for actual drawing.
 * @param W
 */
void RunLogic::drawingThread(DrawWindow *W) {
	if (running && Ava->connected && currentShapeIndex < stopIndex) {
		if (shapes->at(currentShapeIndex)->isBreakPoint){
			toggleBreakPoint(currentShapeIndex);
			running = false;
			return;
		}
		shapes->at(currentShapeIndex)->draw(W);
		emit setRunColor(currentShapeIndex, true);
		W->show();
		currentShapeIndex++;

		if (currentShapeIndex == stopIndex) currentShapeIndex--;
		running = false;
	} else {
		while (running && currentShapeIndex < stopIndex) {
			if (shapes->at(currentShapeIndex)->isBreakPoint){
				toggleBreakPoint(currentShapeIndex);
				running = false;
				return;
			}
			shapes->at(currentShapeIndex)->draw(W);
			emit setRunColor(currentShapeIndex, true);
			W->show();
			currentShapeIndex++;
			_sleep(COMMAND_DELAY);
		}
		if (currentShapeIndex == stopIndex) currentShapeIndex--;
		running = false;
	}
}
