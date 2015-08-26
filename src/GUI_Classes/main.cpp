#include "stdafx.h"

#include "Painter.h"
#include "StartWindow.h"
#include <windows.h>



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//console window//
	AllocConsole();

	SetConsoleTitleA("Robot Artist v3 (8/26/15)");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	//console window//

	//actual program//
	Painter p;
	p.showGUI();
	//actual program//

	return a.exec();
}
