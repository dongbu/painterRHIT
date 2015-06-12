#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Tab Work//
    EditorTabs = new QTabWidget();
    currentEditor = -1;
    tabCount = -1;
    untitledCount = 0;
    ui->EditorTabLayout->addWidget(EditorTabs);
    this->EditorTabs->connect(EditorTabs,QTabWidget::tabCloseRequested, this,MainWindow::closeTab);
    EditorTabs->setTabsClosable(true);
    //Tab work//

    //save work//
    projectName = "Change_Me"; //"garbage" value
    saved = false;
    ui->actionSave->setDisabled(true);//disable save until saveAs has been used.
    //save work//

    //command list//
    ui->listWidget->connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(loadCommandFromList(QListWidgetItem*)));
    ui->MoveUp->connect(ui->MoveUp,QPushButton::clicked,this,MainWindow::MoveUp_clicked);
    ui->MoveDown->connect(ui->MoveDown,QPushButton::clicked,this,MainWindow::MoveDown_clicked);
    ui->DeleteCommand->connect(ui->DeleteCommand,QPushButton::clicked,this,MainWindow::DeleteCommand_clicked);
    //command list//

    //misc work//
    //misc work//
}

/*
 * Default close function (when you press "X").
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * load command when clicked in commandViewer
 */
void MainWindow::loadCommandFromList(QListWidgetItem* item){
    GuiLoadSave::updateCommandEditor(item->text(),projectName,editors.at(currentEditor)->CommandEditorWidget);
}

/*
 * delete item from commandViewer
 */
void MainWindow::DeleteCommand_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

/*
 * move item in commandViewer down
 */
void MainWindow::MoveDown_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    int listSize = ui->listWidget->model()->rowCount();
    if(currentRow >= (listSize - 1)){
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow + 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow + 1);
}

/*
 * move item in commandViewer up
 */
void MainWindow::MoveUp_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if(currentRow == 0){
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget->takeItem(currentRow);
    ui->listWidget->insertItem(currentRow - 1, currentItem);
    ui->listWidget->setCurrentRow(currentRow - 1);
}

/*
 * allows save as functionality.
 */
void MainWindow::on_actionSave_As_triggered()
{
    QFileDialog saveDirectory;
    saveDirectory.setDirectory("ProjectFiles");
    saveDirectory.setAcceptMode(QFileDialog::AcceptSave);
    QStringList filters;
    filters << "Text files (*.txt)";
    saveDirectory.setNameFilters(filters);

    if(saveDirectory.exec()){
        QString name = saveDirectory.selectedFiles().at(0).split("/").last();
        std::cout << name.toStdString() <<std::endl;
                if (!name.isEmpty() && name.isSimpleText() && !name.contains(QRegExp("[" + QRegExp::escape("\\/:*?\"<>|") + "]"))){
                    //creates proper folders
                    int loadReturnCode = GuiLoadSave::createProjectDirectory(name);
                    //return code 0 means it worked.
                    if(loadReturnCode != 0){
                        //ProjectFiles folder could not be created
                        if(loadReturnCode == 1){
                            alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                            alert.setInformativeText("Failed To Create ProjectFiles directory");
                            alert.show();
                            return;
                        }
                        //project already exists
                        else if(loadReturnCode == 2){
                            std::cout << "confirm overwrite" << std::endl;
                            QMessageBox confirmSaveBox;
                            confirmSaveBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);

                            confirmSaveBox.setText("Confirm Overwrite");
                            confirmSaveBox.setButtonText(0,"overwrite");
                            if(confirmSaveBox.exec()){

                                std::cout << "overwrite comfirmed" << std::endl;
                                QDir dir(name);
                                if(dir.exists(name)){
                                    if(!dir.removeRecursively()){
                                        std::cout << "failed to remove previous folder" <<std::endl;
                                        return;
                                    }
                                }
                                saved = true;
                                projectName = name;
                                this->setWindowTitle(projectName);
                                ui->actionSave->setEnabled(true);
                                foreach(CommandEditor *edits, editors){
                                    edits->setProjectName(projectName);
                                }
                                if(!GuiLoadSave::writeCommandListToFolder(projectName,ui->listWidget)){
                                    alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                                    alert.setInformativeText("Failed To Save " + projectName);
                                    alert.show();
                                }
                                //adds a "dummy" txt file
                                QFile dummy;
                                dummy.setFileName(QString("ProjectFiles/") + projectName + QString("/") + name + QString(".txt"));
                                dummy.open(QIODevice::WriteOnly);
                                dummy.close();

                            }
                        }

                    } else{
                        saved = true;
                        projectName = name;
                        this->setWindowTitle(projectName);
                        ui->actionSave->setEnabled(true);
                        foreach(CommandEditor *edits, editors){
                            edits->setProjectName(projectName);
                        }

                        //chunks in index.xml file
                        if(!GuiLoadSave::writeCommandListToFolder(projectName, ui->listWidget)){
                            alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                            alert.setInformativeText("Failed To Create " + projectName + "/index.xml");
                            alert.show();
                        }
                        QFile dummy;
                        dummy.setFileName(QString("ProjectFiles/") + projectName + QString("/") + name + QString(".txt"));
                        dummy.open(QIODevice::WriteOnly);
                        dummy.close();
                    }


                }else{
                    //basically user put in a bad filename
                    alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                    alert.setInformativeText(name + " is not a valid name");
                    alert.show();
                }
    }

}

/*
 * open functionality
 */
void MainWindow::on_actionOpen_triggered()
{
    if(!QDir(QString("ProjectFiles")).exists()){
        if(!QDir().mkdir(QString("ProjectFiles"))){
            std::cout << "failed to create ProjectFiles folder" <<std::endl;
            return;
        }
    }

    QFileDialog directory;
    directory.setDirectory("ProjectFiles");
    QStringList filters;
    filters << "Text files (*.txt)";
    directory.setNameFilters(filters);
    if(directory.exec()){


    projectName = directory.selectedFiles().at(0).split("/").last();
    projectName.chop(4);
    std::cout << projectName.toStdString() << std::endl;
    if(!GuiLoadSave::loadCommandListFromFolder(projectName,ui->listWidget)){
        alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
        alert.setInformativeText("Failed To Load " + projectName + "/index.xml");
        alert.show();
    }else{
        this->setWindowTitle(projectName);
        saved=true;
        ui->actionSave->setEnabled(true);
    }
    }
}


/*
 * save functionality
 */
void MainWindow::on_actionSave_triggered()
{
    if(!GuiLoadSave::writeCommandListToFolder(projectName,ui->listWidget)){
        alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
        alert.setInformativeText("Failed To Save " + projectName);
        alert.show();
    }
}

/*
 * Allows the insertion of a point map command into
 * the command editor
 */
void MainWindow::on_actionDraw_Point_Map_triggered()
{
    CommandEditor *editor = new CommandEditor(ui->listWidget);
    editor->setProjectName(projectName);
    editors.push_back(editor);
    currentEditor++;
    tabCount++;
    untitledCount++;
    EditorTabs->addTab(editor->CommandEditorWidget,"PointMap (" + QString::number(untitledCount) + ")");
    EditorTabs->setCurrentIndex(tabCount);
}

/*
 * allows tabs in command editor to close themsleves
 */
void MainWindow::closeTab(int index) {
    tabCount--;
    EditorTabs->removeTab(index);
}
