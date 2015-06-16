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
    projectName = ""; //"garbage" value
    saved = false;
    fileChanged = false;
    ui->actionSave->setDisabled(true);//disable save until saveAs has been used.
    ui->pushButton->setDisabled(true);//can't load external file until saved.
    //save work//

    //command list//
    ui->listWidget->connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_EditCommand_clicked()));
    ui->MoveUp->connect(ui->MoveUp,QPushButton::clicked,this,MainWindow::MoveUp_clicked);
    ui->MoveDown->connect(ui->MoveDown,QPushButton::clicked,this,MainWindow::MoveDown_clicked);
    ui->DeleteCommand->connect(ui->DeleteCommand,QPushButton::clicked,this,MainWindow::DeleteCommand_clicked);
    //command list//

    //interpreter work//
    interpreter = new CommandInterpreter("");
    //interpreter work//

}


/**
 * @brief Default close function (when you press "X").
 * Also prompts to save or discard unsaved work.
 * Cannot prevent exiting out once begun.
 */
MainWindow::~MainWindow()
{
    //if the file was changed and not saved, asks you if you would like to save
    //or discard changes.
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);

        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            if(saved){
            MainWindow::on_actionSave_triggered();
            }else{
                MainWindow::on_actionSave_As_triggered();
            }
            if(QDir("ProjectFiles/Temp").exists()){
               QDir("ProjectFiles/Temp").removeRecursively();
            }
            break;
        case QMessageBox::Discard:
            if(QDir("ProjectFiles/Temp").exists()){
               QDir("ProjectFiles/Temp").removeRecursively();
            }
            break;
        case QMessageBox::Default:
            //If the user manages to click some other button, return.
            return;
        }
    }
    delete ui;
}


/**
 * @brief slot to delete item from commandViewer
 */
void MainWindow::DeleteCommand_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->currentRow());
    fileChanged = true;
}


/**
 * @brief slot to move item in commandViewer down
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
    fileChanged = true;
}


/**
 * @brief slot to move item in commandViewer up
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
    fileChanged = true;
}


/**
 * @brief Save As functionality.
 */
void MainWindow::on_actionSave_As_triggered()
{


    //make sure ProjectFiles folder exists
    if(!saved){
        if(!QDir(QString("ProjectFiles")).exists()){
            if(!QDir().mkdir(QString("ProjectFiles"))){
                std::cout << "failed to create ProjectFiles folder" <<std::endl;
                return;
            }
        }

        //saveAsProject() returns the name that was chosen to save the project under.
        QString name = GuiLoadSave::saveAsProject();
        if(!name.isEmpty()){
            saved = true;
            projectName = name;
            this->setWindowTitle(projectName);
            interpreter->setProjectName(projectName);
            ui->actionSave->setEnabled(true);
            ui->pushButton->setEnabled(true);
            //updates the commandEditors.
            foreach(CommandEditor *edits, editors){
                edits->setProjectName(projectName);
            }

            //chunks in index.xml file
            if(!GuiLoadSave::writeCommandListToFolder(projectName, ui->listWidget)){
                alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                alert.setInformativeText("Failed To Create " + projectName + "/index.xml");
                if(alert.exec()){
                    return;
                }
            }
            //creates the "dummy" file that is used for clicking.
            QFile dummy;
            dummy.setFileName(QString("ProjectFiles/") + projectName + QString("/") + name + QString(".txt"));
            dummy.open(QIODevice::WriteOnly);
            dummy.close();
            fileChanged = false;

            //moves all files from temp folder into current folder if temp folder exists.  Also deletes temp folder.
            if(QDir("ProjectFiles/Temp").exists()){
               if(!GuiLoadSave::copyAllFilesFrom("Temp",projectName)){
                   std::cout << "Problem with Temp" << std::endl;
               }else{
                   QDir("ProjectFiles/Temp").removeRecursively();
               }
            }
        }
    }else{
        //has been saved before.
        QString prevProjectName = projectName;
        //same as above.
        QString name = GuiLoadSave::saveAsProject();

        if(!name.isEmpty()){
            saved = true;
            ui->pushButton->setEnabled(true);
            projectName = name;
            this->setWindowTitle(projectName);
            ui->actionSave->setEnabled(true);
            interpreter->setProjectName(projectName);

            //updates commandEditors
            foreach(CommandEditor *edits, editors){
                edits->setProjectName(projectName);
            }

            //chunks in index.xml file
            if(!GuiLoadSave::writeCommandListToFolder(projectName, ui->listWidget)){
                alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                alert.setInformativeText("Failed To Create " + projectName + "/index.xml");
                if(alert.exec()){
                    return;
                }
            }
            //creates the "dummy" file that is used for clicking.
            QFile dummy;
            dummy.setFileName(QString("ProjectFiles/") + projectName + QString("/") + name + QString(".txt"));
            dummy.open(QIODevice::WriteOnly);
            dummy.close();

            //transfers all files over from the previous location to the new location.
            if(!GuiLoadSave::copyAllFilesFrom(prevProjectName, projectName)){
                std::cout << "something went wrong transfering files from " <<prevProjectName.toStdString() << " to " << projectName.toStdString() << std::endl;
            }else{
                fileChanged = false;
            }
        }

    }
}


/**
 * @brief opening project functionality
 */
void MainWindow::on_actionOpen_triggered()
{
    //asks you if you want to save your work before opening something new.
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);
        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            if(saved){
                MainWindow::on_actionSave_triggered();
            }else{
                MainWindow::on_actionSave_As_triggered();
            }
            if(QDir("ProjectFiles/Temp").exists()){
               QDir("ProjectFiles/Temp").removeRecursively();
            }
            break;
        case QMessageBox::Discard:
            if(QDir("ProjectFiles/Temp").exists()){
               QDir("ProjectFiles/Temp").removeRecursively();
            }
            break;
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Default:
            //should not get here
            return;
        }
    }
    if(!QDir(QString("ProjectFiles")).exists()){
        if(!QDir().mkdir(QString("ProjectFiles"))){
            std::cout << "failed to create ProjectFiles folder" <<std::endl;
            return;
        }
    }

    //opens up a directory viewer that only shows folders and .txt files.
    QFileDialog directory;
    directory.setDirectory("ProjectFiles");
    QStringList filters;
    filters << "Text files (*.txt)";
    directory.setNameFilters(filters);
    if(directory.exec()){
    MainWindow::cleanUp();

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
        ui->pushButton->setEnabled(true);
        interpreter->setProjectName(projectName);
    }
    }
}


/**
 * @brief normal save functionality
 */
void MainWindow::on_actionSave_triggered()
{
    if(!GuiLoadSave::writeCommandListToFolder(projectName,ui->listWidget)){
        alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
        alert.setInformativeText("Failed To Save " + projectName);
        alert.show();
    }
    fileChanged = false;
}


/**
 * @brief Allows the insertion of a point map command into
 * the command editor
 */
void MainWindow::on_actionDraw_Point_Map_triggered()
{
    tabCount++;
    CommandEditor *editor = new CommandEditor(ui->listWidget,tabCount, EditorTabs);
    editor->setProjectName(projectName);
    editors.push_back(editor);
    currentEditor++;

    untitledCount++;
    EditorTabs->addTab(editor->CommandEditorWidget,"PointMap (" + QString::number(untitledCount) + ")");
    EditorTabs->setCurrentIndex(tabCount);

    //connection so that we know when something has been changed.
    connect(editor,editor->fileStatusChanged,this,fileChangedTrue);


}


/**
 * @brief closes the indecated commandEditor tab.
 * @param index
 */
void MainWindow::closeTab(int index) {
    tabCount--;
    EditorTabs->removeTab(index);
}


/**
 * @brief brings up a new commandEditor with preloaded information.
 */
void MainWindow::on_EditCommand_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item == NULL){
        return;
    }
    QString tempProjectName = projectName;

    //adds new instance of editor
    MainWindow::on_actionDraw_Point_Map_triggered();
    //retrieves most recent editor
    CommandEditor *newEditor = editors.at(currentEditor);
    EditorTabs->setTabText(tabCount,item->text());
    //populates said editor with information loaded via xml.
    GuiLoadSave::updateCommandEditor(item->text(),tempProjectName,newEditor);
}

/**
 * @brief removes all project specific variables and clears away
 * tabs and lists.
 */
void MainWindow::cleanUp(){
    EditorTabs->clear();
    tabCount = -1;
    untitledCount = 0;
    currentEditor = -1;
    ui->listWidget->clear();
    editors.clear();
    this->fileChanged = false;
    this->saved = false;
    this->setWindowTitle("Untitled");
    this->projectName = "";
    interpreter->setProjectName("");
    ui->actionSave->setDisabled(true);
    if(QDir("ProjectFiles/Temp").exists()){
       QDir("ProjectFiles/Temp").removeRecursively();
    }
    ui->pushButton->setDisabled(true);

}


/**
 * @brief slot that sets fileChanged to true
 */
void MainWindow::fileChangedTrue(){
    fileChanged = true;
}


/**
 * @brief slot for a new project
 */
void MainWindow::on_actionNew_triggered()
{
    //asks you if you want to save your work before starting something new.
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);
        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            if(saved){
                MainWindow::on_actionSave_triggered();
            }else{
                MainWindow::on_actionSave_As_triggered();
            }
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Default:
            //should not get here
            return;
        }
    }
    MainWindow::cleanUp();
}

/**
 * @brief slot for adding a command not shown in list
 */
void MainWindow::on_pushButton_clicked()
{

    if(GuiLoadSave::loadExternalFile(projectName,ui->listWidget)){
        this->fileChanged = true;
    }

}

/**
 * @brief slot for starting or continuing painting.
 */
void MainWindow::on_actionRun_triggered()
{
    //temporarily disabled.
//    interpreter->beginPaintingCommands(ui->listWidget);
    //only included for demonstrational purposes.
    Painter *p = new Painter();
    p->paintCommand(0,0,300,300,"blue","solid");

}
