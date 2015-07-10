#ifndef COMMANDVIEWER_H
#define COMMANDVIEWER_H

#include "Line.h"
#include "ui_commandviewer.h"
#include <QWidget>
#include <QListWidget>
#include <QCloseEvent>
#include <iostream>
#include <QXmlStreamReader>
#include <QDebug.h>
#include <QMainWindow>
#include "commandinterpreter.h"

namespace Ui {
class CommandViewer;
}

class CommandViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandViewer(QWidget *parent = 0);
    ~CommandViewer();
    QListWidget *list;
    //std::vector<Line*> editors;
    void clear();

    int PopulateCommandEditor(QString fileName);
    void setProjectName(QString *projectName);
	void setProjectLocation(QString *projectLocation);
    void setMainClosed(bool closed);
    void MakeEditor();
	Line *currentEditor;
    CommandInterpreter *interpreter;
	bool *saved;
	bool fileChanged;


private:
    void ConnectToolBar();
	int FillEditor(QString editorName);
	void runFrom();
	void runOnly();
	void setBreakpoint();

    Ui::CommandViewer *ui;
    void PassFileChange(bool *val);
    QString *projectName, *projectLocation;
	bool mainClosed, freshlyMade;

private slots:
    void MoveUp_clicked();
    void MoveDown_clicked();
    void DeleteCommand_clicked();
    void on_EditCommand_clicked();
    void Add_Command_Clicked();
    void Stop_triggered();
    void Pause_triggered();
    void StepForward_triggered();
    void StepBackwards_triggered();
    void RunFromStart_triggered();
	void launchRightClick(QPoint pos);
	void menuSort(QAction *a);

public slots:
    void fileSaved(bool saved);

signals:
    void fileStatusChanged();
    void triggerPointMap();
    void triggerCommandEditorUpdate(QString, QString*, Line*);
    void Add_External_Command();
    void EmitConnectEditor(Line*);
	void MustSave();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // COMMANDVIEWER_H
