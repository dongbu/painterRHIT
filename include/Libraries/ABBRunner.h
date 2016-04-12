#pragma once

#include "stdafx.h"
#include "abbhelper.h"
#include <Windows.h>

namespace Ui {
	class ABBRunner;
}

class ABBRunner: public QDialog
{
	Q_OBJECT

public:
	ABBRunner(int width, int height);
	~ABBRunner();

	bool next();
	void end();
	bool sendCoord(int x, int y);
	void decidePaint(int r, int g, int b);

	void setSize(int w, int h);

	void abort();

	bool colorUsed [6]; //true if using color in spot
	int colorR[6];
	int colorG[6];
	int colorB[6];

	bool connected;
	void connectWin();



private:
	ABBHelper *helps;
	bool sendSerial(std::string message);
	bool getSerialResponse();
	bool connectToSerial(int port);
	bool sendCanvasInfo();
	bool changeColor(int colorNum);

	HANDLE hSerial;

	int width;
	int height;

private slots:
	void acceptedWin();
	void canceledWin();


};

