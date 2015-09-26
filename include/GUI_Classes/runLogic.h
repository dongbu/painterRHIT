#ifndef RUNLOGIC_H
#define RUNLOGIC_H

#include "stdafx.h"
#include "DrawWindow.cpp"
#include "Shapes.cpp"
#include "CytonRunner.h"

class RunLogic : public QObject

{
    Q_OBJECT

	enum STEP{
		NEITHER, //previously 2  For when neither back nor forward pressed || if back or forward hit their respective limits
		FORWARD, //previously 1  For when forward was successfully pressed
		BACKWARD, //previously 0 For when backward was successfully pressed
	};

public:
    RunLogic(int width, int height, Shapes *shapes, CytonRunner *Ava);
	
	int width, height;

	void hideSimWin();
	void clearSimWin();
	void resetSimWin(int width, int height, std::string string);
	void showSimWin();
	
private:
	void paintWOFeedbackThread(DrawWindow *W);
	void simulateDelayedBrushThread(DrawWindow *W);

	QString mode;
    volatile bool running;
    volatile int currentShapeIndex, stopIndex;
    Shapes *shapes;
	int COMMAND_DELAY;
	CytonRunner *Ava;

	DrawWindow *simWin;

	void paintPolyline(DrawWindow *W, Shape *s, bool simulated);
	void paintFill(DrawWindow *W, Shape *s, bool simulated);

	STEP stepTaken;

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
