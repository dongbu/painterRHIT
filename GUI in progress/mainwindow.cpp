#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);

    QStringList list;
    list << "cats" << "dogs" << "birds";
    model->setStringList(list);

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    //insert
    int row = ui->listView->currentIndex().row();
    model->insertRows(row,1);

    QModelIndex index = model->index(row);

    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
}

void MainWindow::on_pushButton_3_clicked()
{
    //delete
    model->removeRow(ui->listView->currentIndex().row());
}

void MainWindow::on_pushButton_4_clicked()
{
    //move down
//    int currentRow = ui->listView->currentIndex().row();
//    if (currentRow == 0) return;
//    ui->listView->model()
//    QListViewItem *currentItem = ui->listView->takeItem(currentRow);
//    ui->listView->insertItem(currentRow - 1, currentItem);
//    ui->listView->setCurrentRow(currentRow - 1);
}

void MainWindow::on_pushButton_clicked()
{
    //move up
//    int currentRow = ui->listView->currentRow();
//    if (currentRow > listView->count()-1) return;
//    QListVidgetItem *currentItem = ui->listView->takeItem(currentRow);
//    ui->listView->insertItem(currentRow + 1, currentItem);
//    ui->listView->setCurrentRow(currentRow + 1);
}
