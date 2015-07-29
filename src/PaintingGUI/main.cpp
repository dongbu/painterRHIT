//#include <QApplication>
#include <windows.h>
#include "Painter.h"
#include "Shapes.cpp"
#include "DrawWindow.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AllocConsole();

    SetConsoleTitleA("Robot Artist v2 (7/29/15)");
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);

    Painter *painter = new Painter();
    painter->showGUI(true);

    return a.exec();
}
