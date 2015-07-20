#include "RunLogic.h"

int COMMAND_DELAY = 1000; //(ms)

/**
 * @brief constructor
 * @param width
 * @param height
 * @param shapes
 */
RunLogic::RunLogic(int width, int height, Shapes *shapes) {
    this->width = width;
    this->height = height;
    this->shapes = shapes;
    this->simWin = new DrawWindow(width, height, "simulation window");
    this->simWin->hideWindow();
    stopClicked();
}

/**
 * @brief updates the stop index.
 */
void RunLogic::shapesChanged() { stopIndex = shapes->length(); }

/**
 * @brief stops and clears simulation.
 */
void RunLogic::stopClicked() {
    this->simWin->showWindow();
    running = false;
    simWin->clearWindow(255, 255, 255); //white
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
    currentShapeIndex++;
}
/**
 * @brief steps backwards.
 */
void RunLogic::backwardClicked() {
    this->simWin->showWindow();
    emit setRunColor(currentShapeIndex, false);
    if (currentShapeIndex <= 0) return;
    currentShapeIndex--; //otherwise, -- the index
    simWin->clearWindow(255, 255, 255); //white
    running = false;
    for (int i = 0; i < currentShapeIndex; i++) runOnly(i);
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
    while (running && currentShapeIndex < stopIndex) {
        if (shapes->at(currentShapeIndex)->isBreakPoint){
            printf("hit a breakpoint\n");
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
    running = false;
}
