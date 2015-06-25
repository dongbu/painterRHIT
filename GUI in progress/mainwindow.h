#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "commandinterpreter.h"
#include "guiloadsave.h"
#include "commandeditor.h"
#include "CommandViewer.h"
#include "drawonwidget.h"

#include <QtGui>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidget>
#include <QFile>
#include <vector>
#include <QScrollArea>
#include <iostream>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QListWidget>
#include <QFileDialog>
#include <QThread>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void recievePoint(int x, int y, int pointCount);

private slots:
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    void on_actionDraw_Point_Map_triggered();
    void closeTab(int index);
    void on_actionSave_triggered();
    void fileChangedTrue();
    void on_actionNew_triggered();
    void on_pushButton_clicked();
    void on_actionRun_triggered();
    void noticeCommandAdded(int index);

    void on_actionStop_triggered();

    void on_actionPause_triggered();

    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void tabChanged(int index);
    void callUpdate(QString fileName,QString ProjectName, CommandEditor* loadedEditor);


private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    QString projectName;
    bool saved;
    QMessageBox alert;
    bool fileChanged;
    CommandInterpreter *interpreter;
    drawOnWidget *drawOn;

    std::vector<CommandEditor*> editors;
    QTabWidget *EditorTabs;
    int currentEditor;
    int tabCount;
    int untitledCount;

    void changeCurrentEditor(QString editorName);
    void cleanUp();

    void startPainting(QListWidget* widget, int index);

    CommandViewer *commandView;

protected:
    void closeEvent(QCloseEvent *);

};

#endif // MAINWINDOW_H
