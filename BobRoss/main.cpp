//#include <QApplication>
#include <iostream>
#include "Painter.h"
#include "shapes.cpp"
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Painter p;

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	
	cv::RNG rng(12345);
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
		p.addShape(PL);
	}
	p.launchSimulatorWindow();
    return a.exec();
}
