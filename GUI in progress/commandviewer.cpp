#include "commandviewer.h"
#include "ui_commandviewer.h"
#include <QToolBar>
#include <QAction>
#include <qlistwidget.h>

CommandViewer::CommandViewer(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::CommandViewer)
{
	//window setup
	ui->setupUi(this);
	list = ui->listWidget;
	this->setWindowTitle("Command List");

	//List Set-Up
	ui->MoveUp->connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(MoveUp_clicked()));
	ui->MoveDown->connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(MoveDown_clicked()));
	ui->DeleteCommand->connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(DeleteCommand_clicked()));
	ui->AddCommand->connect(ui->AddCommand, SIGNAL(clicked()), this, SLOT(Add_Command_Clicked()));
	ui->AddCommand->setEnabled(false);
	mainClosed = false;
	freshlyMade = true;

	//Simulator Set-Up
	interpreter = new CommandInterpreter("");
	this->Stop_triggered();
	this->ConnectToolBar();
	this->move(0, 500);
	this->show();
	this->saved = false;
	this->fileChanged = false;

}

/**
 * @brief initially populate the debugging toolbar
 */
void CommandViewer::ConnectToolBar() {
	connect(ui->Pause, SIGNAL(triggered()), this, SLOT(Pause_triggered()));
	connect(ui->Stop, SIGNAL(triggered()), this, SLOT(Stop_triggered()));
	connect(ui->StepBackwards, SIGNAL(triggered()), this, SLOT(StepBackwards_triggered()));
	connect(ui->StepForwards, SIGNAL(triggered()), this, SLOT(StepForward_triggered()));
	connect(ui->RunFromStart, SIGNAL(triggered()), this, SLOT(RunFromStart_triggered()));
}

/**
 * @brief Stops simulation.
 */
void CommandViewer::Stop_triggered()
{
	ui->Pause->setDisabled(true);
	ui->Stop->setDisabled(true);
	ui->StepBackwards->setDisabled(true);
	//ui->StepForwards->setDisabled(true);
	interpreter->stopPaintingCommands();
}

/**
 * @brief Pauses simulation.
 */
void CommandViewer::Pause_triggered()
{
	interpreter->pausePaintingCommands();
}

/**
 * @brief steps simulation forwards.
 */
void CommandViewer::StepForward_triggered()
{
	if (fileChanged || !saved){
		emit MustSave();
	}
	if (saved){
		ui->StepBackwards->setEnabled(true);
		interpreter->setList(list);
		interpreter->stepForwardCommands();
	}
}

/**
 * @brief steps simulation backwards
 */
void CommandViewer::StepBackwards_triggered()
{
	interpreter->stepBackwardCommands();
}

/**
 * @brief Starts/continues simulation.
 */
void CommandViewer::RunFromStart_triggered()
{
	if (fileChanged || !saved){
		emit MustSave();
	}
	if (saved){
		//currently always starts from beginning.
		ui->Pause->setEnabled(true);
		ui->Stop->setEnabled(true);
		ui->StepBackwards->setEnabled(true);
		ui->StepForwards->setEnabled(true);
		interpreter->setProjectName(*projectName);
		interpreter->setList(list);
		interpreter->beginPaintingCommands(0);
	}
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
	if (currentRow >= (listSize - 1)){
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
	if (currentRow == 0){
		return;
	}
	QListWidgetItem *currentItem = list->takeItem(currentRow);
	list->insertItem(currentRow - 1, currentItem);
	list->setCurrentRow(currentRow - 1);
	emit fileStatusChanged();
}

void CommandViewer::setProjectName(QString *projectName) {
	this->projectName = projectName;
	this->interpreter->setProjectName(*projectName);
}


/**
 * @brief brings up a new Line with preloaded information.
 */
void CommandViewer::on_EditCommand_clicked()
{
	QListWidgetItem *selectedCommand = list->currentItem();
	if (selectedCommand == NULL){
		return; //check that command is selected
	}
	if (currentEditor != NULL && freshlyMade == false) {
		currentEditor->close(); //close old window
	}
	freshlyMade = false;
	//Make new editor && populate from XML
	MakeEditor();
	FillEditor(list->currentItem()->text());
	currentEditor->setCommandAdded(true);
	currentEditor->show();
}

void CommandViewer::closeEvent(QCloseEvent *event){
	if (mainClosed){
		event->accept();
	}
	else{
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
	this->saved = &saved;
}

/**
 * @brief Allows the insertion of a point map command into
 * the command editor
 */
void CommandViewer::MakeEditor()
{
	Line *editor = new Line();
	editor->setList(list);
	editor->setProjectName(*this->projectName);

	//searches through and sets the default name to 1 + the largest.
	editor->setName("PointMap_1");
	QList<QListWidgetItem *> listOfCommands = list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
	QList<QString> texts;
	foreach(QListWidgetItem *item, listOfCommands)
		texts.append(item->text());

	int k = 1;
	while (texts.contains(editor->getName())){
		editor->setName("PointMap_" + QString::number(k));
		k++;
	}

	currentEditor = editor;
	emit EmitConnectEditor(editor);

}

/**
* @brief puts info from xml into command editor
* @param toPopulate
* @return 0 if successful
*/
int CommandViewer::FillEditor(QString editorName)
{
	//Filling in editor's points
	QList<QLineEdit *> lineEdits = currentEditor->CommandEditorWidget->findChildren<QLineEdit *>();

	lineEdits.first()->setDisabled(true);

	QString tempProjectName;
	if (projectName->isEmpty() || projectName->isNull()){
		tempProjectName = "Temp";
	}
	else{
		tempProjectName = *projectName;
	}

	//load file and set up the reader.
	QFile loadFile;
	loadFile.setFileName(QString("ProjectFiles/") + tempProjectName + QString("/") + editorName + QString(".xml"));
	loadFile.open(QIODevice::ReadOnly);
	QXmlStreamReader reader(&loadFile);

	//skip over unimportant doc headers.
	reader.readNextStartElement();
	reader.readNextStartElement();

	int i = 0;

	//get list of comboBoxes
	QList<QComboBox *> comboBoxes = currentEditor->CommandEditorWidget->findChildren<QComboBox *>();

	QList<QSpinBox *> spinBoxes = currentEditor->CommandEditorWidget->findChildren<QSpinBox *>();

	//set text for the command name slot.
	lineEdits.at(0)->setText(editorName);

	//set the information for both comboboxes.
	foreach(const QXmlStreamAttribute &attr, reader.attributes()){
		if (i < comboBoxes.length()){
			comboBoxes.at(i)->setCurrentIndex(comboBoxes.at(i)->findText(attr.value().toString()));
		}
		else{
			spinBoxes.at(0)->setValue(attr.value().toInt());
		}
		i++;
	}

	int k = 2;

	//keep going until the document is finished.
	while (!reader.isEndDocument()){
		reader.readNext();
		if (reader.isStartElement()){
			QString pointString = "";
			if (reader.name().toString() == "PointMap"){
				//find and add the correct number of points.
				int numPoints = reader.attributes().at(0).value().toString().toInt();
				for (int j = 0; j < (numPoints - 2); j++){
					currentEditor->Add_Point_Clicked();

				}
				lineEdits = currentEditor->CommandEditorWidget->findChildren<QLineEdit *>();
			}
			if (reader.name().toString() == "Point"){
				foreach(const QXmlStreamAttribute &attr, reader.attributes()){
					pointString.append(attr.value().toString());
					pointString.append(",");
				}
				pointString.chop(1);

				currentEditor->CommandEditorWidget->findChildren<QLineEdit *>().at(k)->setText(pointString);
				k++;

			}
			else if (reader.name().toString() == "FileMalformed"){
				if (reader.attributes().value(0).toString() == "1"){
					std::cout << "FILE WAS MALFORMED!" << std::endl;
					//potentially highlight poorly made files.
				}
			}
		}
	}

	if (reader.hasError()){
		std::cout << "there was an error in reading the file" << std::endl;
		//shouldn't be an issue, but put in just in case.
	}

	loadFile.close();
	return 0;
}


/**
 * @brief removes all project specific variables and clears away
 * lists.
 */
void CommandViewer::clear() {
	this->setProjectName(new QString(""));
	currentEditor = NULL;
	list->clear();
	interpreter->clear();
}