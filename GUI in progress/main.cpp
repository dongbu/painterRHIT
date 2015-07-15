#include "mainwindow.h"
#include "painter.h"
#include <QApplication>

/**
 * @brief launches the application
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    //loads up the main window.  DO NOT TOUCH!
    QApplication a(argc, argv);
	//temporary write things in console//
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	//temporary write things in console//
	painter p;
	std::string var;
	//p.setGuiEditable(true);
	printf("toggling ");
    //MainWindow w;
    //w.show();

    return a.exec();
}
