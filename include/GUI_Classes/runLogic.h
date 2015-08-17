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
	void DrawingThread(DrawWindow *W, CytonRunner *Ava);

	QString mode;
    volatile bool running;
    volatile int currentShapeIndex, stopIndex;
    Shapes *shapes;
	int width, height, COMMAND_DELAY, stepTaken;
	CytonRunner *Ava;

public slots:
	void updateMode(QString mode, int delay);
    void clearClicked();
    void pauseClicked();
    void forwardClicked();
    void backwardClicked();
    void runClicked();
    void runFrom(int index);
    void runOnly(int index);
    void shapesChanged();
	void reset();

signals:
	void updateCommandList(int index, QString runToggle);
};

#endif // RUNLOGIC_H
