//#include <QApplication>
#include <iostream>
#include "Painter.h"
#include "Shapes.cpp"
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	AllocConsole();
	SetConsoleTitleA("Robot Artist V2.0.0 (7/16/15)");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);


    return a.exec();
}
