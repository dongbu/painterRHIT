#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);

    //to do with command editor
    QScrollArea *scrollArea = new QScrollArea();

    scrollArea->setParent(ui->Command_Editor_Frame);

    scrollArea->setWidget(ui->Parameter);
    ui->Parameter->show();
    scrollArea->show();
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    numberOfPoints = 2;

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

void MainWindow::on_pushButton_5_clicked()
{
    numberOfPoints++;
    QString s = ("Point " + QString::number(numberOfPoints) + ":");
    ui->ParameterHolder->addRow(new QLabel(s),new QLineEdit("0,0"));
}

void MainWindow::on_toolButton_clicked()
{
    if(ui->Command_Editor_Frame->isVisible()){
        ui->Command_Editor_Frame->hide();
    } else{
        ui->Command_Editor_Frame->show();
    }
}
