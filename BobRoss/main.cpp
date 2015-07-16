//#include <QApplication>
#include <iostream>
#include "Painter.h"
#include "shapes.cpp"
#include <windows.h>
#include "DrawWindow.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	
    return a.exec();
}
