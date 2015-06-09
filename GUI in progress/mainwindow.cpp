#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollArea>
#include <iostream>
#include <QListWidget>

using namespace std;

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
    //end of command editor work

    //to do with command list
    QStringList list;
    list << "cats" << "dogs" << "birds";
    model->setStringList(list);

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    //end of command list work

    //to do with tab manager
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->setTabsClosable(true);

    //end of tab manager work
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_2_clicked()
//{
//    //insert
//    int row = ui->listView->currentIndex().row();
//    model->insertRows(row,1);

//    QModelIndex index = model->index(row);

//    ui->listView->setCurrentIndex(index);
//    ui->listView->edit(index);
//}

void MainWindow::on_pushButton_3_clicked()
{
    //delete
    model->removeRow(ui->listView->currentIndex().row());
}

void MainWindow::on_pushButton_4_clicked()
{
    //move down
    ui->tabWidget->addTab(ui->Command_Editor_Frame,"check");
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

//hides and shows the command editor frame
void MainWindow::on_toolButton_clicked()
{
    if(ui->Command_Editor_Frame->isVisible()){
        ui->Command_Editor_Frame->hide();
    } else{
        ui->Command_Editor_Frame->show();
    }
}
//adds point to command editor
void MainWindow::on_AddPointButton_clicked()
{
    numberOfPoints++;
    QString s = ("Point " + QString::number(numberOfPoints) + ":");
    ui->ParameterHolder->addRow(new QLabel(s),new QLineEdit("0,0"));
    ui->RemovePointButton->setEnabled(true);
}
//removes point from command editor
void MainWindow::on_RemovePointButton_clicked()
{
    if(numberOfPoints > 2){
        numberOfPoints--;
        if(numberOfPoints == 2){
            ui->RemovePointButton->setDisabled(true);
        }
        int lastInput = ui->ParameterHolder->count() - 1;
        int secondLast = lastInput - 1;
        QLayoutItem *toDelete1 = ui->ParameterHolder->itemAt(lastInput);
        QLayoutItem *toDelete2 = ui->ParameterHolder->itemAt(secondLast);
        ui->ParameterHolder->removeItem(toDelete1);
        ui->ParameterHolder->removeItem(toDelete2);
        toDelete1->widget()->setVisible(false);
        toDelete2->widget()->setVisible(false);
    }
}

//adds command from command editor into command list
void MainWindow::on_actionDraw_Point_Map_triggered()
{
    QListWidget *toAdd = new QListWidget(this);
    ui->tabWidget->addTab(toAdd,"hi");
}

//button that doesn't do anything yet.  (to add command).
void MainWindow::on_AddCommandButton_clicked()
{

}

//Routes the "x-clicked on tab" signal.
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
