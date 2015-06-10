#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "guiloadsave.h"
#include <QScrollArea>
#include <iostream>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QListWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //to do with saving
    projectName = "Change_Me"; //"garbage" value
    saved = false;

    ui->actionSave->setDisabled(true);//disable save until saveAs has been used.

    //end of saving


    ///to do with command editor
//    QScrollArea *scrollArea = new QScrollArea(); //scroll area stuff.  not pleasant.

//    scrollArea->setParent(ui->Command_Editor_Frame);

//    scrollArea->setWidget(ui->Parameter);
//    ui->Parameter->show();
//    scrollArea->show();
//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    numberOfPoints = 2;
//    ui->RemovePointButton->setDisabled(true);
    currentEditor = 0;
    editors.push_back(new CommandEditor("New Command",ui->listWidget));

    CommandEditor *temp = editors.at(currentEditor);
    temp->setParent(this);
    ui->CommandEditLayout->addWidget(temp->CommandEditorWidget);

    ///end of command editor work

    //to do with command list
    ui->listWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    ui->listWidget->connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(loadCommandFromList(QListWidgetItem*)));
    //end of command list work
}

//default delete function
MainWindow::~MainWindow()
{
    delete ui;
}

//load command when clicked in commandViewer
void MainWindow::loadCommandFromList(QListWidgetItem* item){
    GuiLoadSave::updateCommandEditor(item->text(),projectName,editors.at(currentEditor)->CommandEditorWidget);


}

//remove item from commandViewer
void MainWindow::on_pushButton_3_clicked()
{
    //delete
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

//move item in commandViewer down
void MainWindow::on_pushButton_4_clicked()
{
    //    move down
    int currentRow = ui->listWidget->currentRow();
    int listSize = ui->listWidget->model()->rowCount();
    if(currentRow >= (listSize - 1)){
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow + 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow + 1);
}

//move item in commandViewer up
void MainWindow::on_pushButton_clicked()
{
    //move up
    int currentRow = ui->listWidget->currentRow();
    if(currentRow == 0){
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow - 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow - 1);
}

//hides and shows the command editor frame
void MainWindow::on_toolButton_clicked()
{
    if(editors.at(currentEditor)->CommandEditorWidget->isVisible()){
        editors.at(currentEditor)->CommandEditorWidget->hide();
    } else{
        editors.at(currentEditor)->CommandEditorWidget->show();
    }
}

void MainWindow::changeCurrentEditor(QString editorName){
    std::cout << "implement later" <<std::endl;
}

////adds point to command editor
//void MainWindow::on_AddPointButton_clicked()
//{
//    //fairly self explanatory.  Adds point options
//    numberOfPoints++;
//    QString s = ("Point " + QString::number(numberOfPoints) + ":");
//    ui->ParameterHolder->addRow(new QLabel(s),new QLineEdit("0,0"));
//    ui->RemovePointButton->setEnabled(true);
//    ui->ParameterHolder->update();
//    ui->Parameter->update();
//}

//removes point from command editor
//void MainWindow::on_RemovePointButton_clicked()
//{
//    //will not remove first two points
//    if(numberOfPoints > 2){
//        numberOfPoints--;
//        if(numberOfPoints == 2){
//            ui->RemovePointButton->setDisabled(true);
//        }
//        //last two items (paired together)
//        int lastInput = ui->ParameterHolder->count() - 1;
//        int secondLast = lastInput - 1;
//        QLayoutItem *toDelete1 = ui->ParameterHolder->itemAt(lastInput);
//        QLayoutItem *toDelete2 = ui->ParameterHolder->itemAt(secondLast);
//        //removes with gusto.  Probably some overkill here.
//        ui->ParameterHolder->removeItem(toDelete1);
//        ui->ParameterHolder->removeItem(toDelete2);
//        toDelete1->widget()->setVisible(false);
//        toDelete2->widget()->setVisible(false);
//        toDelete1->widget()->deleteLater();
//        toDelete2->widget()->deleteLater();
//        ui->ParameterHolder->update();
//        ui->Parameter->update();
//    }
//}

//adds command from command editor into command list and saves the command as an .xml file
//void MainWindow::on_AddCommandButton_clicked()
//{
//    //checks to make sure it has been saved. ie. ensures projectName is not a garbage variable.
//    if(!saved){
//        alert.setText("");
//        alert.setInformativeText("Must Save Or Select Project Before Adding Commands");
//        alert.show();
//        return;
//    }
//    //saves the command and outputs a message if it was malformed.  Still saves even if malformed.
//    if(!GuiLoadSave::writeCommandToFolder(projectName,ui->Parameter,ui->listWidget)){
//        alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
//        alert.setInformativeText("File Malformed");
//        alert.show();
//        return;
//    }

//}


//allows saveas functionality.
void MainWindow::on_actionSave_As_triggered()
{
    if(saved){
        std::cout << "will figure out eventually" << std::endl;
    }else{
        //brings up a box for you to put in the name of your project
        bool ok;
        QString name = saveProjectWindow.getText(NULL, "Please name your project", NULL, QLineEdit::Normal, "projectName", &ok);
        //checks to make sure the input is good
        if (ok && !name.isEmpty() && name.isSimpleText() && !name.contains(QRegExp("[" + QRegExp::escape("\\/:*?\"<>|") + "]"))){
            int loadReturnCode = GuiLoadSave::createProjectDirectory(name);
            //return code 0 means it worked.
            if(loadReturnCode != 0){
                alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                if(loadReturnCode == 1){
                alert.setInformativeText("Folder Creation Failed");
                }else if(loadReturnCode == 2){
                    alert.setInformativeText("Project Name Already Taken!");
                }
                alert.show();
            } else{
                saved = true;
                projectName = name;
                this->setWindowTitle(projectName);
                foreach(CommandEditor *edits, editors){
                    edits->setProjectName(projectName);
                }
            }
        }else{
            //basically user put in a bad filename
            alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
            alert.setInformativeText(name + " is not a valid name");
            alert.show();
        }


    }

}


//open functionality TODO
void MainWindow::on_actionOpen_triggered()
{
}


