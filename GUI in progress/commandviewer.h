#ifndef COMMANDVIEWER_H
#define COMMANDVIEWER_H

#include "CommandEditor.h"
#include <QWidget>
#include <QListWidget>
#include <QCloseEvent>


namespace Ui {
class CommandViewer;
}

class CommandViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CommandViewer(QWidget *parent = 0);
    ~CommandViewer();
    QListWidget *list;

    void infoDump(QString *projectName, std::vector<CommandEditor*> *editors, int *currentEditor, int tabCount,QTabWidget *EditorTabs);
    void setMainClosed(bool closed);

private:
    Ui::CommandViewer *ui;
    void PassFileChange(bool *val);
    QString *projectName;
    std::vector<CommandEditor*> *editors;
    int *currentEditor;
    int tabCount;
    QTabWidget *EditorTabs;
    bool mainClosed;

private slots:
    void MoveUp_clicked();
    void MoveDown_clicked();
    void DeleteCommand_clicked();
    void on_EditCommand_clicked();

signals:
    void fileStatusChanged();
    void triggerPointMap();
    void triggerCommandEditorUpdate(QString, QString*, CommandEditor*);

protected:
    void closeEvent(QCloseEvent *);
};

#endif // COMMANDVIEWER_H
