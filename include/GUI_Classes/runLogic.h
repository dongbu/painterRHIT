#ifndef RUNLOGIC_H
#define RUNLOGIC_H

#include "stdafx.h"
#include "DrawWindow.cpp"
#include "Shapes.cpp"
#include "CytonRunner.h"

class RunLogic : public QObject

{
    Q_OBJECT

public:
    RunLogic(int width, int height, Shapes *shapes, CytonRunner *Ava);
	DrawWindow *simWin;

private:
	void DrawingThread(DrawWindow *W);
    volatile bool running;
    volatile int currentShapeIndex, stopIndex;
    Shapes *shapes;
    int width, height;
	CytonRunner *Ava;
	int stepTaken; //0 for back, 1 for forward, 2 for neither.

public slots:
    void stopClicked();
    void pauseClicked();
    void forwardClicked();
    void backwardClicked();
    void runClicked();
    void runFrom(int index);
    void runOnly(int index);
    void toggleBreakPoint(int index);
    void shapesChanged();
	void reset();

signals:
    void setRunColor(int index, QString runToggle);
    void clearRunColors();
};

#endif // RUNLOGIC_H
