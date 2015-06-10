#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidget>
#include <QFile>

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
    void on_AddPointButton_clicked();

    void on_RemovePointButton_clicked();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_toolButton_clicked();

    void on_AddCommandButton_clicked();

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
    void updateCommandEditor(QString);


};

#endif // MAINWINDOW_H
