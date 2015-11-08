#include "stdafx.h"

#include "Painter.h"
#include "StartWindow.h"
#include <windows.h>
#include <algorithm>


bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//!!
	//There is a memory leak. never, ever run this without debug parameters
	//So why is it even here?!@?
	//if (cmdOptionExists(argv, argv + argc, "-debug")){

	//console window//
	AllocConsole();

	SetConsoleTitleA("Robot Artist v3 (11/8/15)");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	//console window//
	//}


	//actual program//
	Painter p;
	p.showGUI();
	//actual program//

	return a.exec();
}
