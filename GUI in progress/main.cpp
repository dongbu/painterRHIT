#include "mainwindow.h"
#include "painter.h"
#include "Line.h"
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
	p.setGuiEditable(true);
	p.load();
	Line *l = new Line();
	l->AddPoint(110, 110);
	l->AddPoint(10, 710);
	l->AddPoint(710, 10);
	p.addLine(l);
	p.setGuiEditable(true);
    //MainWindow w;
    //w.show();

    return a.exec();
}
