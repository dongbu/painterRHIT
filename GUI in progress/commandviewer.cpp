#include "commandviewer.h"
#include "ui_commandviewer.h"
#include <iostream>

CommandViewer::CommandViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommandViewer)
{
    ui->setupUi(this);
    this->list = ui->listWidget;
    this->show();

    ui->listWidget->connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_EditCommand_clicked()));
    ui->MoveUp->connect(ui->MoveUp,SIGNAL(clicked()),this,SLOT(MoveUp_clicked()));
    ui->MoveDown->connect(ui->MoveDown,SIGNAL(clicked()),this,SLOT(MoveDown_clicked()));
    ui->DeleteCommand->connect(ui->DeleteCommand,SIGNAL(clicked()),this,SLOT(DeleteCommand_clicked()));

    mainClosed = false;
}

CommandViewer::~CommandViewer()
{
    delete ui;
}


/**
 * @brief slot to delete item from commandViewer
 */
void CommandViewer::DeleteCommand_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->currentRow());
    emit fileStatusChanged();
}


/**
 * @brief slot to move item in commandViewer down
 */
void CommandViewer::MoveDown_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    int listSize = ui->listWidget->model()->rowCount();
    if(currentRow >= (listSize - 1)){
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow + 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow + 1);
    emit fileStatusChanged();
}


/**
 * @brief slot to move item in commandViewer up
 */
void CommandViewer::MoveUp_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if(currentRow == 0){
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow - 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow - 1);
    emit fileStatusChanged();
}

void CommandViewer::infoDump(QString *projectName, std::vector<CommandEditor*> *editors, int *currentEditor, int tabCount,QTabWidget *EditorTabs) {
    this->projectName = projectName;
    this->editors = editors;
    this->currentEditor = currentEditor;
    this->tabCount = tabCount;
    this->EditorTabs = EditorTabs;
}


/**
 * @brief brings up a new commandEditor with preloaded information.
 */
void CommandViewer::on_EditCommand_clicked()
{
    QListWidgetItem *item = this->list->currentItem();
    if(item == NULL){
        return;
    }
    QString *tempProjectName = projectName;

    //adds new instance of editor
    emit triggerPointMap();

    //retrieves most recent editor
    CommandEditor *newEditor = editors->at(*currentEditor);
    EditorTabs->setTabText(tabCount,item->text());
    //populates said editor with information loaded via xml.
    emit triggerCommandEditorUpdate(item->text(),tempProjectName,newEditor);
}

void CommandViewer::closeEvent(QCloseEvent *event){
        if(mainClosed){
            event->accept();
        }else{
            event->ignore();
            this->lower();

        }
}

void CommandViewer::setMainClosed(bool closed){
    this->mainClosed = closed;
}
