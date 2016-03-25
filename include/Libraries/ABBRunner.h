#pragma once

#include "stdafx.h"
#include "abbhelper.h"

namespace Ui {
	class ABBRunner;
}

class ABBRunner: public QDialog
{
	Q_OBJECT

public:
	ABBRunner();
	~ABBRunner();

	bool sendCoord(int x, int y);
	bool changeColor(int colorNum);
	bool connectToSerial(int port);
	bool setColors();
	void abort(std::string fI);

	bool colorUsed [6]; //true if using color in spot
	int colorR[6];
	int colorG[6];
	int colorB[6];

private:
	ABBHelper *helps;

private slots:
	void acceptedWin();
	void canceledWin();


};

