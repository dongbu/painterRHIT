//#include <QApplication>
#include <iostream>
#include "Painter.h"
#include "Shapes.cpp"
#include <windows.h>
#include "DrawWindow.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	AllocConsole();
	SetConsoleTitleA("Robot Artist V2.0.0 (7/16/15)");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	cv::RNG rng(12345);

	Shapes S;

	// add some lines
	int w = 400;
	int h = 500;
	PolyLine *PL;
	for (int i = 0; i<5; i++) {
		PL = new PolyLine();
		PL->setPenColor(rng.uniform(50, 250), rng.uniform(50, 250), rng.uniform(50, 250));
		for (int p = 0; p<10; p++) {
			PL->addPoint(rng.uniform(100, w - 100), rng.uniform(100, h - 100));
		}
		//    S.addPolyLine(PL);
		S.addShape(PL);
	}

	// add a poly point region
	PolyPoints *PP = new PolyPoints();
	int ww = 100;
	int delx = 20;
	int dely = 200;
	PP->setPenColor(255, 255, 0);
	PP->addPoint(ww / 4.0 + delx, 7 * ww / 8.0 + dely);
	PP->addPoint(3 * ww / 4.0 + delx, 7 * ww / 8.0 + dely);
	PP->addPoint(3 * ww / 4.0 + delx, 13 * ww / 16.0 + dely);
	PP->addPoint(11 * ww / 16.0 + delx, 13 * ww / 16.0 + dely);
	PP->addPoint(19 * ww / 32.0 + delx, 3 * ww / 8.0 + dely);
	PP->addPoint(3 * ww / 4.0 + delx, 3 * ww / 8.0 + dely);
	PP->addPoint(3 * ww / 4.0 + delx, ww / 8.0 + dely);
	PP->addPoint(26 * ww / 40.0 + delx, ww / 8.0 + dely);
	PP->addPoint(26 * ww / 40.0 + delx, ww / 4.0 + dely);
	PP->addPoint(22 * ww / 40.0 + delx, ww / 4.0 + dely);
	PP->addPoint(22 * ww / 40.0 + delx, ww / 8.0 + dely);
	PP->addPoint(18 * ww / 40.0 + delx, ww / 8.0 + dely);
	PP->addPoint(18 * ww / 40.0 + delx, ww / 4.0 + dely);
	PP->addPoint(14 * ww / 40.0 + delx, ww / 4.0 + dely);
	PP->addPoint(14 * ww / 40.0 + delx, ww / 8.0 + dely);
	PP->addPoint(ww / 4.0 + delx, ww / 8.0 + dely);
	PP->addPoint(ww / 4.0 + delx, 3 * ww / 8.0 + dely);
	PP->addPoint(13 * ww / 32.0 + delx, 3 * ww / 8.0 + dely);
	PP->addPoint(5 * ww / 16.0 + delx, 13 * ww / 16.0 + dely);
	PP->addPoint(ww / 4.0 + delx, 13 * ww / 16.0 + dely);
	S.addShape(PP);

	// add pixel region
	PixelRegion *PR = new PixelRegion();
	PR->setPenColor(0, 0, rng.uniform(0, 244));
	for (int i = 0; i<100; i++) {
		PR->addPoint(rng.uniform(0, w), rng.uniform(h - 100, h));
	}
	S.addShape(PR);
	//////////////////////////////////
	Painter *p = new Painter(S);
	p->launchSimulatorWindow();
	p->launchCommandWindow();
>>>>>>> 4ad9dbacacf4c074f10852e034502d6dd605bfef

    return a.exec();
}
