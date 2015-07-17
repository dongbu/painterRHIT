#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include "ui_commandwindow.h"
#include "EditorWindow.h"
#include "DrawWindow.cpp"

namespace Ui {
class CommandWindow;
}

class CommandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandWindow(QWidget *parent = 0);
    ~CommandWindow();
    void setShapes(Shapes shapes);
	void setSimWindow(DrawWindow *sim);
	Shapes shapes;

private:
    void launchEditorWin(int index);
    void runFrom(int index);
    void runOnly(int index);
    void setBreakPoint(int index);
	void populate();

    Ui::CommandWindow *ui;
	DrawWindow *simWin;
    EditorWindow *currentEditor;

private slots:
    void moveUpClicked();
    void moveDownClicked();
    void deleteCommandClicked();

	void launchRightClick(); //Zach has refused to do this

	//waiting for sim. to take time
    void stopClicked();
    void pauseClicked();
    void forwardClicked();
    void backwardClicked();
    void runClicked();
	//waiting for sim. to take time


public slots:
    void addCommand();

signals:
	//waiting for sim. to take time
    void pause(); 
    void stop();
    void forward();
    void backward();
    void run();
	//waiting for sim. to take time

};

#endif // COMMANDWINDOW_H

