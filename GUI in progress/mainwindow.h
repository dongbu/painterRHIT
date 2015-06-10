#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "commandeditor.h"
#include <QtGui>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidget>
#include <QFile>
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_toolButton_clicked();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();

    void loadCommandFromList(QListWidgetItem*);

private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    int numberOfPoints;
    QString projectName;
    bool saved;
    QMessageBox alert;
    QInputDialog saveProjectWindow;
    std::vector<CommandEditor*> editors;
    int currentEditor;
    //CommandEditor *editor;

    void changeCurrentEditor(QString editorName);


};

#endif // MAINWINDOW_H
