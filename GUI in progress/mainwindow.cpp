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
    this->EditorTabs->connect(EditorTabs,SIGNAL(tabCloseRequested(int)), this,SLOT(closeTab(int)));
    EditorTabs->setTabsClosable(true);
    connect(EditorTabs,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
    //Tab work//

    //save work//
    projectName = ""; //"garbage" value
    saved = false;
    fileChanged = false;
    ui->pushButton->setDisabled(true);//can't load external file until saved.
    //creates a ProjectFiles folder if one doesn't exist
    if(!QDir(QString("ProjectFiles")).exists()){
        if(!QDir().mkdir(QString("ProjectFiles"))){
            std::cout << "failed to create ProjectFiles folder" <<std::endl;
        }
    }
    //save work//

    //command list//
    commandView = new CommandViewer();
    commandView->infoDump(projectName,editors,currentEditor,tabCount,EditorTabs);
    connect(commandView,SIGNAL(triggerPointMap()),this,SLOT(on_actionDraw_Point_Map_triggered()));
    //command list//

    //interpreter work//
    interpreter = new CommandInterpreter("");
    //interpreter work//

    //click to draw work//
    drawOn = new drawOnWidget(ui->widget);
    drawOn->setFixedWidth(1000);
    drawOn->setFixedHeight(750);
    connect(drawOn,SIGNAL(sendPoint(int, int, int)),this,SLOT(recievePoint(int, int, int)));
    ui->widget->setStyleSheet("background-color:white;");
    ///temporarily disabled until further discussion///
    ui->actionDraw_Circle->setDisabled(true);
    ui->actionDraw_Function->setDisabled(true);
    ui->actionDraw_Line->setDisabled(true);
    ui->actionDraw_Point_Map->setDisabled(true);
    ui->actionDraw_Square->setDisabled(true);
    ui->DrawFunctions->setHidden(true);
    ///temporarily disabled until further discussion///
    //click to draw work//

    //debug button work//
    ui->actionPause->setDisabled(true);
    ui->actionPrevious->setDisabled(true);
    ui->actionStop->setDisabled(true);
    ui->actionNext->setDisabled(true);
    //debug button work//

}


/**
 * @brief Default after close all functions (when you press "X").
 */
MainWindow::~MainWindow()
{

    delete ui;
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
            interpreter->setProjectName(projectName);
            this->setWindowTitle(projectName);
            interpreter->setProjectName(projectName);
            ui->actionSave->setEnabled(true);
            ui->pushButton->setEnabled(true);
            //updates the commandEditors.
            foreach(CommandEditor *edits, editors){
                edits->setProjectName(projectName);
            }

            //chunks in index.xml file
            if(!GuiLoadSave::writeCommandListToFolder(projectName, this->commandView->list)){
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
            interpreter->setProjectName(projectName);
            this->setWindowTitle(projectName);
            ui->actionSave->setEnabled(true);
            interpreter->setProjectName(projectName);

            //updates commandEditors
            foreach(CommandEditor *edits, editors){
                edits->setProjectName(projectName);
            }

            //chunks in index.xml file
            if(!GuiLoadSave::writeCommandListToFolder(projectName, this->commandView->list)){
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
    interpreter->setProjectName(projectName);
    if(!GuiLoadSave::loadCommandListFromFolder(projectName,this->commandView->list)){
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
    if(!saved){
        MainWindow::on_actionSave_As_triggered();
        return;
    }
    if(!GuiLoadSave::writeCommandListToFolder(projectName,this->commandView->list)){
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
    CommandEditor *editor = new CommandEditor(this->commandView->list,tabCount, EditorTabs);
    editor->setProjectName(projectName);

    //searches through and sets the default name to 1 + the largest.
    editor->setName("PointMap_1");
    QList<QListWidgetItem *> listOfCommands = this->commandView->list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QList<QString> texts;
    foreach(QListWidgetItem *item, listOfCommands)
      texts.append(item->text());
    int k = 1;
    while(texts.contains(editor->getName())){
        editor->setName("PointMap_" + QString::number(k));
        k++;
    }


    editors.push_back(editor);
    currentEditor++;

    untitledCount++;
    EditorTabs->addTab(editor->CommandEditorWidget,"PointMap (" + QString::number(untitledCount) + ")");
    EditorTabs->setCurrentIndex(tabCount);

    //connections so that we know when something has been changed.
    connect(editor,SIGNAL(fileStatusChanged()),this,SLOT(fileChangedTrue()));
    connect(commandView,SIGNAL(fileStatusChanged()),this,SLOT(fileChangedTrue()));

//    //connection so we know if addcommand button was pressed.
//    connect(editor,SIGNAL(tell_Command_Added(int)),this,SLOT(noticeCommandAdded(int)));

//    //connection so we know if a tab was closed.  clears drawer if a tab is closed.
//    connect(EditorTabs,SIGNAL(tabCloseRequested(int)),this,SLOT(noticeCommandAdded(int)));

    //connection to update drawOn.
    connect(editor,SIGNAL(sendUpdateToDrawOn(CommandEditor*)),drawOn,SLOT(updateToEditor(CommandEditor*)));




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
 * @brief removes all project specific variables and clears away
 * tabs and lists.
 */
void MainWindow::cleanUp(){
    EditorTabs->clear();
    tabCount = -1;
    untitledCount = 0;
    currentEditor = -1;
    this->commandView->list->clear();
    editors.clear();
    this->fileChanged = false;
    this->saved = false;
    this->setWindowTitle("Untitled");
    this->projectName = "";
    interpreter->setProjectName(projectName);
    interpreter->setProjectName("");
    interpreter->clear();
    ui->actionSave->setDisabled(true);
    if(QDir("ProjectFiles/Temp").exists()){
       QDir("ProjectFiles/Temp").removeRecursively();
    }
    ui->pushButton->setDisabled(true);
    drawOn->clearAll();

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

    if(GuiLoadSave::loadExternalFile(projectName,this->commandView->list)){
        this->fileChanged = true;
    }

}

/**
 * @brief slot for starting or continuing painting.
 */
void MainWindow::on_actionRun_triggered()
{
    if(fileChanged || !saved){
        MainWindow::on_actionSave_triggered();
    }
    if(saved){
        //currently always starts from beginning.
        ui->actionPause->setEnabled(true);
        ui->actionStop->setEnabled(true);
        ui->actionPrevious->setEnabled(true);
        ui->actionNext->setEnabled(true);
        interpreter->setProjectName(projectName);
        interpreter->beginPaintingCommands(this->commandView->list, 0);

    }



}

/**
 * @brief slot for DrawOnWidget to get points that were clicked.
 * @param x
 * @param y
 */
void MainWindow::recievePoint(int x, int y, int pointCount){
    //means command is over.
    if(x == -10 && y == -10){
        if(pointCount >= 3){
           CommandEditor *temp = editors.at(currentEditor);
           temp->add_Command_Externally();
           return;
        }else{
            drawOn->clearAll();
            return;
        }
    }
    if(pointCount == 1){
        this->on_actionDraw_Point_Map_triggered();
    }
    CommandEditor *temp = editors.at(currentEditor);
    if(pointCount > 2){
        temp->Add_Point_Clicked();
    }
    temp->set_Point_At(pointCount, x, y);

}

void MainWindow::noticeCommandAdded(int index){
    if(index == (tabCount+1) || index == -10){
    drawOn->clearAll();
    }
}


void MainWindow::on_actionStop_triggered()
{
    ui->actionPause->setDisabled(true);
    ui->actionStop->setDisabled(true);
    ui->actionPrevious->setDisabled(true);
    interpreter->stopPaintingCommands();

}

void MainWindow::on_actionPause_triggered()
{
    interpreter->pausePaintingCommands();
}

void MainWindow::on_actionNext_triggered()
{
    interpreter->stepForwardCommands();
}



void MainWindow::on_actionPrevious_triggered()
{
    interpreter->stepBackwardCommands();

}

void MainWindow::tabChanged(int index){
   if(index >= 0){
       currentEditor = index;
       CommandEditor *temp = editors.at(currentEditor);
       temp->InfoChanged();
   }

}

void MainWindow::callUpdate(QString fileName,QString ProjectName, CommandEditor* loadedEditor) {
    GuiLoadSave::updateCommandEditor(fileName,ProjectName,loadedEditor);
}

/**
 * @brief closes all the windows when this window is closed.
 * also prompts for a save.
 */
void MainWindow::closeEvent(QCloseEvent *event){
    //if the file was changed and not saved, asks you if you would like to save
    //or discard changes.
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
            event->accept();
            break;
        case QMessageBox::Discard:
            if(QDir("ProjectFiles/Temp").exists()){
               QDir("ProjectFiles/Temp").removeRecursively();
            }
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        case QMessageBox::Default:
            //If the user manages to click some other button, return.
            event->ignore();
            return;
        }
    }
    commandView->close();
}
