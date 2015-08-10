#include "stdafx.h"

#include "Painter.h"
#include "StartWindow.h"
#include <windows.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	StartWindow w;
	w.show();

	//console window//
    AllocConsole();

    SetConsoleTitleA("Robot Artist v3 (8/10/15)");
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);

    //Painter *painter = new Painter();
    //painter->showGUI(true);
	//painter->loadPhoto("../../images/lena.jpg");
	//console window//

    return a.exec();
}
