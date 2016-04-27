#pragma once

#include "stdafx.h"
#include "abbhelper.h"
#include "Shapes.cpp"
#include <Windows.h>

namespace Ui {
	class ABBRunner;
}

class ABBRunner: public QDialog
{
	Q_OBJECT

public:
	ABBRunner(int width, int height, Shapes *shape);
	~ABBRunner();

	bool next();
	void end();
	bool sendCoord(int x, int y);
	bool decidePaint(char col);

	void setSize(int w, int h);

	void abort();

	Shapes *shape;

	bool connected;
	void connectWin();



private:
	ABBHelper *helps;
	bool sendSerial(std::string message);
	bool getSerialResponse();
	bool connectToSerial(int port);
	bool sendCanvasInfo();

	HANDLE hSerial;

	int width;
	int height;

private slots:
	void acceptedWin();
	void canceledWin();


};

