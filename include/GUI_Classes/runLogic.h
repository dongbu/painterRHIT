#ifndef RUNLOGIC_H
#define RUNLOGIC_H

#include "stdafx.h"
#include "DrawWindow.cpp"
#include "Shapes.cpp"
#include "CytonRunner.h"
#include "ABBRunner.h"

class RunLogic : public QObject

{
    Q_OBJECT

	enum STEP{
		NEITHER, //For when neither back nor forward pressed || if back or forward hit their respective limits
		FORWARD, //For when forward was successfully pressed
		BACKWARD, //For when backward was successfully pressed
	};

public:
    RunLogic(int width, int height, Shapes *shapes, CytonRunner *Ava, ABBRunner *chappie);
	
	int width, height;

	void hideSimWin();
	void clearSimWin();
	void resetSimWin(int width, int height, std::string string);
	void showSimWin();

	void resetStepTaken();
	
private:
	void paintThread(DrawWindow *W);

	QString mode;
    volatile bool running;
    volatile int currentShapeIndex, stopIndex;
    Shapes *shapes;
	int COMMAND_DELAY;
	CytonRunner *Ava;
	ABBRunner *chappie;

	DrawWindow *simWin;

	void paintFill(DrawWindow *W, Shape *s);
	void setAvaPenColor(Shape *s);
	void doStroke(std::vector<cv::Point> pts, DrawWindow *W, bool ignoreSmall = false);
	void drawPolyLine(std::vector<cv::Point> pts, DrawWindow *W);

	STEP stepTaken;

	int straighten(std::vector<cv::Point> pts, int index);
	bool endCheck(std::vector<cv::Point> pts, int index);
	double angleDiff(cv::Point p1, cv::Point p2);
	

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
