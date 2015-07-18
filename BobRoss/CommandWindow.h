#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include "ui_commandwindow.h"
#include "EditorWindow.h"

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
	Ui::CommandWindow *ui;

private slots:
    void moveUpClicked();
    void moveDownClicked();
    void deleteCommandClicked();
	void launchRightClick(QPoint p);
	void menuSort(QAction *a);

public slots:
	void populate();

signals:
	void modifiedCommand();
	void runFrom(int index);
	void runOnly(int index);
	void setBreakPoint(int index);

};

#endif // COMMANDWINDOW_H

