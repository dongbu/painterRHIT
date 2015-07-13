#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "commandinterpreter.h"
#include "guiloadsave.h"
#include "Line.h"
#include "CommandViewer.h"
#include "drawonwidget.h"
#include "WebcamHandler.h"
#include "WorkSpace.h"

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
#include <qspinbox.h>
#include <qaction.h>


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
	void saveTempIndex();

private slots:
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void fileChangedTrue();
	void fileChangedFalse();
    void on_actionNew_triggered();
    void noticeCommandAdded(int index);
    void ConnectEditor(Line* editor);
	void on_actionConnect_triggered();
    void on_drawing_changed();
	void drawOn2_update();
    void openCamera();

private:
    Ui::MainWindow *ui;
    bool saved;
    QMessageBox alert;
    bool fileChanged, editorWorks;
    drawOnWidget *drawOn, *drawOn2;
	WorkSpace *workSpace;
    CommandViewer *commandView;
    QComboBox *colorBox, *styleBox;
    QSpinBox *thicknessBox;
	WebcamHandler cam;
	

    void cleanUp();
    void startPainting(QListWidget* widget, int index);

protected:
    void closeEvent(QCloseEvent *);

signals:
	void sendSaved(bool saved);
    void makeConnection(QString name);
    void sendLineStyles(QString color, QString style, int width);
	void sendListOfCommands(CommandViewer *commandView);

};

#endif // MAINWINDOW_H
