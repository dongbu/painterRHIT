#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include "stdafx.h"
#include "CytonRunner.h"
#include "../src/PaintingGUI/Shapes.cpp"

namespace Ui {
class CommandWindow;
}

class CommandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandWindow(Shapes *ss, QWidget *parent = 0);
    ~CommandWindow();
    Shapes *shapes;
	CytonRunner *Ava;
    Ui::CommandWindow *ui;

private slots:
    void moveUpClicked();
    void moveDownClicked();
    void deleteCommandClicked();
    void launchRightClick(QPoint p);
    void menuSort(QAction *a);

public slots:
    void populate();
    void recieveBreakPointColor(int index, bool toggle);
    void recieveRunColor(int index, bool toggle);
    void recieveClearRunColors();
	void recieveRobot(CytonRunner *Ava);

signals:
    void modifiedCommand();
    void runFrom(int index);
    void runOnly(int index);
    void setBreakPoint(int index);

};

#endif // COMMANDWINDOW_H

