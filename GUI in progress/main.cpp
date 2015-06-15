#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //loads up the main window.  DO NOT TOUCH!
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
