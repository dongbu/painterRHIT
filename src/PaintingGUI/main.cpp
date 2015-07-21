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
    SetConsoleTitleA("Robot Artist V2.0.0 (7/21/15)");
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);

    PolyLine *p1 = new PolyLine();
    p1->addPoint(50, 60);
    p1->addPoint(80, 200);
    PolyLine *p2 = new PolyLine();
    p2->addPoint(300, 300);
    p2->addPoint(400, 400);
    PolyLine *p3 = new PolyLine();
    p3->addPoint(200, 200);
    p3->addPoint(100, 200);
    p3->addPoint(400, 250);
    p3->addPoint(23, 429);
    Shapes *ss = new Shapes();
    ss->addShape(p1);
    ss->addShape(p2);
    ss->addShape(p3);
    Painter *painter = new Painter(ss);
    painter->showGUI(true);

    return a.exec();
}
