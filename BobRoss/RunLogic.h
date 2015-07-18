#ifndef RUNLOGIC_H
#define RUNLOGIC_H

#include "DrawWindow.cpp"
#include "shapes.cpp"

#include <qobject.h>
#include <stdio.h>
#include <iostream>
#include <qobject.h>

class RunLogic : public QObject

{
	Q_OBJECT

public:
	RunLogic(int width, int height, Shapes *shapes);

private:
	void drawingThread(DrawWindow *W);
	volatile bool running;
	volatile int currentShapeIndex, stopIndex;
	Shapes *shapes;
	int width, height;
	DrawWindow *simWin;


public slots:
	void stopClicked();
	void pauseClicked();
	void forwardClicked();
	void backwardClicked();
	void runClicked();
	void runFrom(int index);
	void runOnly(int index);
	void setBreakPoint(int index);
	void shapesChanged();
};

#endif // RUNLOGIC_H