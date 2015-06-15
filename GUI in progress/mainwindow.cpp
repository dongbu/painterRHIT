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
    //save work//

    //command list//
    ui->listWidget->connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_EditCommand_clicked()));
    ui->MoveUp->connect(ui->MoveUp,QPushButton::clicked,this,MainWindow::MoveUp_clicked);
    ui->MoveDown->connect(ui->MoveDown,QPushButton::clicked,this,MainWindow::MoveDown_clicked);
    ui->DeleteCommand->connect(ui->DeleteCommand,QPushButton::clicked,this,MainWindow::DeleteCommand_clicked);
    //command list//

}


/**
 * @brief Default close function (when you press "X").
 * Also prompts to save or discard unsaved work.
 * Cannot prevent exiting out once begun.
 */
MainWindow::~MainWindow()
{
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);
        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            MainWindow::on_actionSave_triggered();
            break;
        case QMessageBox::Discard:
            delete ui;
            break;
        case QMessageBox::Default:
            //should not get here
            return;
        }
    }else{
        delete ui;
    }
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

    QString name = GuiLoadSave::saveAsProject();
    if(!name.isEmpty()){
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
            if(alert.exec()){
                return;
            }
        }
        QFile dummy;
        dummy.setFileName(QString("ProjectFiles/") + projectName + QString("/") + name + QString(".txt"));
        dummy.open(QIODevice::WriteOnly);
        dummy.close();
        fileChanged = false;
    }
    }else{
        QString prevProjectName = projectName;
        QString name = GuiLoadSave::saveAsProject();
        if(!name.isEmpty()){
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
                if(alert.exec()){
                    return;
                }
            }
            QFile dummy;
            dummy.setFileName(QString("ProjectFiles/") + projectName + QString("/") + name + QString(".txt"));
            dummy.open(QIODevice::WriteOnly);
            dummy.close();

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
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);
        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            MainWindow::on_actionSave_triggered();
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
    QString tempProjectName = projectName;

    //adds new instance of editor
    MainWindow::on_actionDraw_Point_Map_triggered();
    //retrieves most recent editor
    CommandEditor *newEditor = editors.at(currentEditor);
    EditorTabs->setTabText(tabCount,item->text());

    GuiLoadSave::updateCommandEditor(item->text(),tempProjectName,newEditor);
}

/**
 * @brief cleans up everything for a "fresh" start.
 */


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

}


/**
 * @brief slot that sets fileChanged to true
 */
void MainWindow::fileChangedTrue(){
    fileChanged = true;
}
