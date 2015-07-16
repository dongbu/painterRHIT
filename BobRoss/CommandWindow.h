#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include "ui_commandwindow.h"
//#include "shapes.cpp"
#include "EditorWindow.h"

//#include <vector>
//#include <QtGui>
//#include <QObject>
//#include <QMainWindow>


namespace Ui {
class CommandWindow;
}

class CommandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandWindow(QWidget *parent = 0);
    ~CommandWindow();
    void setShapes(Shapes *shapes);

private:
    void launchEditorWin(int index);
    void runFrom(int index);
    void runOnly(int index);
    void setBreakPoint(int index);

    Ui::CommandWindow *ui;
    Shapes *shapes;
    EditorWindow currentEditor;

private slots:
    void moveUpClicked();
    void moveDownClicked();
    void deleteCommandClicked();
    void stopClicked();
    void pauseClicked();
    void forwardClicked();
    void backwardClicked();
    void launchRightClick();
    void runClicked();

public slots:
    void addCommand(Shape s);

signals:
    void pause();
    void stop();
    void forward();
    void backward();
    void run();
};

#endif // COMMANDWINDOW_H

