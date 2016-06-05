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
	
	////console window//
	AllocConsole();

	std::string  titleString = "Robot Artist V3 (";
	std::string date = __DATE__;
	std::string titleEnd = ")";

	// attach the new console to this application's process
	AttachConsole(GetCurrentProcessId());

	SetConsoleTitleA((titleString + date + titleEnd).c_str());
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	//actual program//

	try {
		Painter p(argc, argv);
		//actual program//

		p.exec();
	}
	catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
