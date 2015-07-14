#ifndef COMMANDVIEWER_H
#define COMMANDVIEWER_H

#include "Line.h"
#include "ui_commandviewer.h"
#include "WorkSpace.h"
#include "Shape.h"

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
    void clear();
	void MakeEditor();

    void setMainClosed(bool closed);
    CommandInterpreter *interpreter;
	bool *saved;
	bool fileChanged;
	void setWorkSpace(WorkSpace *workSpace);
	Line *currentEditor;

private:
    void ConnectToolBar();
	int FillEditor(QString editorName);
	void runFrom();
	void runOnly();
	void setBreakpoint();

    Ui::CommandViewer *ui;
	bool mainClosed, freshlyMade;
	WorkSpace *workSpace;

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
    void Add_External_Command();
	void MustSave();
	void EmitConnectEditor(Line* currentEditor);

protected:
    void closeEvent(QCloseEvent *);
};

#endif // COMMANDVIEWER_H
