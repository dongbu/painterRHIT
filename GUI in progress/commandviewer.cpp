#include "commandviewer.h"
#include "ui_commandviewer.h"
#include <QToolBar>
#include <QAction>

CommandViewer::CommandViewer(QWidget *parent) :
QMainWindow(parent),
    ui(new Ui::CommandViewer)
{
    ui->setupUi(this);
    list = ui->listWidget;

    ui->MoveUp->connect(ui->MoveUp,SIGNAL(clicked()),this,SLOT(MoveUp_clicked()));
    ui->MoveDown->connect(ui->MoveDown,SIGNAL(clicked()),this,SLOT(MoveDown_clicked()));
    ui->DeleteCommand->connect(ui->DeleteCommand,SIGNAL(clicked()),this,SLOT(DeleteCommand_clicked()));
    ui->AddCommand->connect(ui->AddCommand,SIGNAL(clicked()),this,SLOT(Add_Command_Clicked()));
    ui->AddCommand->setEnabled(false);
    mainClosed = false;

    this->ConnectToolBar();
    this->move(0, 500);
    this->show();
}

/**
 * @brief initially populate the debugging toolbar
 */
void CommandViewer::ConnectToolBar() {
    connect(ui->Pause,SIGNAL(triggered()),this,SLOT(Pause_triggered()));
    connect(ui->Stop,SIGNAL(triggered()),this,SLOT(Stop_triggered()));
    connect(ui->StepBackwards,SIGNAL(triggered()),this,SLOT(StepBackwards_triggered()));
    connect(ui->StepForwards,SIGNAL(triggered()),this,SLOT(StepForward_triggered()));
    connect(ui->RunFromStart,SIGNAL(triggered()),this,SLOT(RunFromStart_triggered()));
}

/**
 * @brief Stops simulation.
 */
void CommandViewer::Stop_triggered()
{
//    ui->actionPause->setDisabled(true);
//    ui->actionStop->setDisabled(true);
//    ui->actionPrevious->setDisabled(true);
//    ui->actionNext->setDisabled(true);
//    interpreter->stopPaintingCommands();
}

/**
 * @brief Pauses simulation.
 */
void CommandViewer::Pause_triggered()
{
//    interpreter->pausePaintingCommands();
}

/**
 * @brief steps simulation forwards.
 */
void CommandViewer::StepForward_triggered()
{
//    interpreter->stepForwardCommands();
}

/**
 * @brief steps simulation backwards
 */
void CommandViewer::StepBackwards_triggered()
{
//    interpreter->stepBackwardCommands();
}

/**
 * @brief Starts/continues simulation.
 */
void CommandViewer::RunFromStart_triggered()
{
//    if(fileChanged || !saved){
//        MainWindow::on_actionSave_triggered();
//    }
//    if(saved){
//        //currently always starts from beginning.
//        ui->actionPause->setEnabled(true);
//        ui->actionStop->setEnabled(true);
//        ui->actionPrevious->setEnabled(true);
//        ui->actionNext->setEnabled(true);
//        interpreter->setProjectName(projectName);
//        interpreter->beginPaintingCommands(this->commandView->list, 0);

//    }
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
    list->takeItem(list->currentRow());
    emit fileStatusChanged();
}


/**
 * @brief slot to move item in commandViewer down
 */
void CommandViewer::MoveDown_clicked()
{
    int currentRow = list->currentRow();
    int listSize = list->model()->rowCount();
    if(currentRow >= (listSize - 1)){
        return;
    }
    QListWidgetItem *currentItem = list->takeItem(currentRow);
    list->insertItem(currentRow + 1, currentItem);
    list->setCurrentRow(currentRow + 1);
    emit fileStatusChanged();
}


/**
 * @brief slot to move item in commandViewer up
 */
void CommandViewer::MoveUp_clicked()
{
    int currentRow = list->currentRow();
    if(currentRow == 0){
        return;
    }
    QListWidgetItem *currentItem = list->takeItem(currentRow);
    list->insertItem(currentRow - 1, currentItem);
    list->setCurrentRow(currentRow - 1);
    emit fileStatusChanged();
}

void CommandViewer::setProjectName(QString *projectName) {
    this->projectName = projectName;
}


/**
 * @brief brings up a new commandEditor with preloaded information.
 */
void CommandViewer::on_EditCommand_clicked()
{
    QListWidgetItem *selectedCommand = list->currentItem();
    if(selectedCommand == NULL){
        return;
    }
    //sets the current editor index
    currentEditor->close();
    currentEditor = editors.at(list->currentIndex().row());

    //makes and fills an editor from xml load file.
    PopulateCommandEditor(list->currentItem()->text());
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

void CommandViewer::Add_Command_Clicked(){
    emit Add_External_Command();
}

void CommandViewer::fileSaved(bool saved){
    ui->AddCommand->setEnabled(saved);
}

/**
 * @brief puts info from xml into command editor
 * @param toPopulate
 * @return 0 if successful
 */
int CommandViewer::PopulateCommandEditor(QString fileName){
    //Creating new editor
    CommandEditor *toPopulate = new CommandEditor();
    toPopulate->show();
    toPopulate->setList(list);
    toPopulate->setProjectName(*projectName);
    emit EmitConnectEditor(toPopulate);
    toPopulate->setName(fileName);
    this->currentEditor = toPopulate;
    toPopulate->commandAdded = true;

    //Filling in editor's points
    QList<QLineEdit *> lineEdits = toPopulate->CommandEditorWidget->findChildren<QLineEdit *>();

    lineEdits.first()->setDisabled(true);

    QString tempProjectName;
    if(projectName->isEmpty() || projectName->isNull()){
        tempProjectName = "Temp";
    }else{
        tempProjectName = *projectName;
    }

    //load file and set up the reader.
    QFile loadFile;
    loadFile.setFileName(QString("ProjectFiles/") + tempProjectName+ QString("/") + fileName + QString(".xml"));
    loadFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&loadFile);

    //skip over unimportant doc headers.
    reader.readNextStartElement();
    reader.readNextStartElement();

    int i = 0;

    //get list of comboBoxes
    QList<QComboBox *> comboBoxes = toPopulate->CommandEditorWidget->findChildren<QComboBox *>();

    //set text for the command name slot.
    lineEdits.at(0)->setText(fileName);

    //set the information for both comboboxes.
    foreach(const QXmlStreamAttribute &attr, reader.attributes()){
        comboBoxes.at(i)->setCurrentIndex(comboBoxes.at(i)->findText(attr.value().toString()));
        i++;
    }

    int k = 1;

    //keep going until the document is finished.
    while(!reader.isEndDocument()){
        reader.readNext();
        if(reader.isStartElement()){
            QString pointString = "";
            if(reader.name().toString() == "PointMap"){
                //find and add the correct number of points.
                int numPoints = reader.attributes().at(0).value().toString().toInt();
                for(int j = 0; j < (numPoints - 2); j++){
                    toPopulate->Add_Point_Clicked();

                }
                lineEdits = toPopulate->CommandEditorWidget->findChildren<QLineEdit *>();
            }
            if(reader.name().toString() == "Point"){
                foreach(const QXmlStreamAttribute &attr, reader.attributes()){
                    pointString.append(attr.value().toString());
                    pointString.append(",");
                }
                pointString.chop(1);

                toPopulate->CommandEditorWidget->findChildren<QLineEdit *>().at(k)->setText(pointString);
                k++;

            } else if(reader.name().toString() == "FileMalformed"){
                if(reader.attributes().value(0).toString() == "1"){
                    std::cout << "FILE WAS MALFORMED!" << std::endl;
                    //potentially highlight poorly made files.
                }
            }
        }
    }

    if(reader.hasError()){
        std::cout << "there was an error in reading the file" <<std::endl;
        //shouldn't be an issue, but put in just in case.
    }

    loadFile.close();
    return 0;
}

/**
 * @brief Allows the insertion of a point map command into
 * the command editor
 */
void CommandViewer::MakeEditor()
{
    CommandEditor *editor = new CommandEditor();
    editor->setList(list);
    editor->setProjectName(*projectName);

    ////deleting previous editor
    //if(list->count() > 0) {
    //    currentEditor->close();
    //}

    //searches through and sets the default name to 1 + the largest.
    editor->setName("PointMap_1");
    QList<QListWidgetItem *> listOfCommands = list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QList<QString> texts;
    foreach(QListWidgetItem *item, listOfCommands)
      texts.append(item->text());

    int k = 1;
    while(texts.contains(editor->getName())){
        editor->setName("PointMap_" + QString::number(k));
        k++;
    }

    editors.push_back(editor);
    currentEditor = editor;

    emit EmitConnectEditor(editor);
}
