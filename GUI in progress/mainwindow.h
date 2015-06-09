#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

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

    void on_AddPointButton_clicked();

    void on_RemovePointButton_clicked();

    void on_AddCommandButton_clicked();

    void on_actionDraw_Point_Map_triggered();

    void on_actionDraw_Point_Map_toggled(bool arg1);

    void on_actionDraw_Point_Map_hovered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    int numberOfPoints;

};

#endif // MAINWINDOW_H
