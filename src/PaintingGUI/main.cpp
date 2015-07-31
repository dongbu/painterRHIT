#include <windows.h>
#include "Painter.h"
#include "Shapes.cpp"
#include "DrawWindow.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//console window//
    AllocConsole();

    SetConsoleTitleA("Robot Artist v2 (7/29/15)");
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);

    Painter *painter = new Painter();
    painter->showGUI(true);
	painter->loadPhoto("../../demos/images/lena.jpg");
	//console window//

    return a.exec();
}
