#include <QApplication>
#include "CommandWindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	CommandWindow *w = new CommandWindow();
	w->show();
    return a.exec();
}
