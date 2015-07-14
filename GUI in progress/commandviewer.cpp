#include "commandviewer.h"
#include "ui_commandviewer.h"
#include <QToolBar>
#include <QAction>
#include <qlistwidget.h>


/**
 * @brief Displays a list of available (already created) painting commands.
 * @param parent
 */
CommandViewer::CommandViewer(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::CommandViewer)
{
	printf("commandViewer loading\n");
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
	interpreter = new CommandInterpreter();
	this->ConnectToolBar();
	this->move(0, 500);
	this->show();
	this->saved = false;
	this->fileChanged = false;

	list->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(launchRightClick(QPoint)));
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
	ui->StepBackwards->setEnabled(true);
	interpreter->stepForwardCommands(list->count());
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
	//currently always starts from beginning.
	ui->Pause->setEnabled(true);
	ui->Stop->setEnabled(true);
	ui->StepBackwards->setEnabled(true);
	interpreter->beginPaintingCommands(0, list->count());
}

/**
 * @brief virtual destructor.
 */
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

/**
* @brief Allows the insertion of a point map command into
* the command editor
*/
void CommandViewer::MakeEditor()
{
	Line *editor = new Line();
	//searches through and sets the default name to 1 + the largest.
	editor->setName("PointMap_1");
	QList<QListWidgetItem *> listOfCommands = list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
	QList<QString> texts;
	foreach(QListWidgetItem *item, listOfCommands){
		texts.append(item->text());
	}

	int k = 1;
	QString currentName = QString("PointMap_") + QString::number(k);
	while (texts.contains(currentName)){
		k++;
		currentName = QString("PointMap_") + QString::number(k);
	}

	editor->setName("PointMap_" + QString::number(k));
	currentEditor = editor;
	emit EmitConnectEditor(editor);
}

/**
 * @brief closing event for commandviewer
 * @param event
 */
void CommandViewer::closeEvent(QCloseEvent *event){
	if (mainClosed){
		event->accept();
	}
	else{
		event->ignore();
		this->lower();
	}
}

/**
 * @brief Sets the variable mainClosed.
 * @param closed
 */
void CommandViewer::setMainClosed(bool closed){
	this->mainClosed = closed;
}

/**
 * @brief emits a signal that the add command button
 * was pressed.
 */
void CommandViewer::Add_Command_Clicked(){
	emit Add_External_Command();
}

/**
 * @brief changes the variable "saved."
 * @param saved
 */
void CommandViewer::fileSaved(bool saved){
	ui->AddCommand->setEnabled(saved);
	this->saved = &saved;
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
	if (workSpace->projectName.isEmpty() || workSpace->projectName.isNull()){
		tempProjectName = "Temp";
	}
	else{
		tempProjectName = workSpace->projectName;
	}
	//load file and set up the reader.
	QFile loadFile;
	loadFile.setFileName(QString(workSpace->projectLocation) + QString("/") + editorName + QString(".xml"));
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

	//keep going until the document is finished.
	while (reader.name().toString() != "FileMalformed"){
		reader.readNext();
		if (reader.isStartElement()){
			QString pointString = "";
			
			if (reader.name().toString() == "Point"){
				foreach(const QXmlStreamAttribute &attr, reader.attributes()){
					pointString.append(attr.value().toString());
					pointString.append(",");
				}
				pointString.chop(1);

				QStringList p = pointString.split(',');
				
				currentEditor->AddPoint(p.at(0).toInt(), p.at(1).toInt());

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
	workSpace->projectName = "";
	workSpace->projectLocation = "ProjectFiles/Temp";
	list->clear();
	interpreter->clear();
}
/**
 * @brief in progress
 * @param pos
 */
void CommandViewer::launchRightClick(QPoint pos) {
	QMenu *menu = new QMenu(list);

	menu->addAction(new QAction("Run from here",list));
	menu->addAction(new QAction("Run this command", list));
	menu->addAction(new QAction("Set Breakpoint", list));

	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSort(QAction*)));

	menu->popup(list->viewport()->mapToGlobal(pos));
}

void CommandViewer::menuSort(QAction *a) {
	if (a->text() == "Run from here") {
		runFrom();
	} else if (a->text() == "Run this command") {
		runOnly();
	}
	else {
		setBreakpoint();
	}
}

void CommandViewer::runFrom() {
	if (fileChanged || !saved){
		emit MustSave();
	}
		ui->StepBackwards->setEnabled(true);
		ui->Stop->setEnabled(true);
		ui->Pause->setEnabled(true);

		interpreter->beginPaintingCommands(list->currentRow(), list->count());
}

void CommandViewer::runOnly() {
	if (fileChanged || !saved){
		emit MustSave();
	}
		ui->StepBackwards->setEnabled(true);
		ui->Stop->setEnabled(true);
		ui->Pause->setEnabled(true);
		interpreter->beginPaintingCommands(list->currentRow(), list->currentRow() + 1);
}

void CommandViewer::setBreakpoint() {

	interpreter->breakPointList.push_back(list->currentRow());
	list->currentItem()->setTextColor(Qt::red);
}

void CommandViewer::setWorkSpace(WorkSpace *workSpace){
	this->workSpace = workSpace;
	this->interpreter->setWorkSpace(workSpace);
	this->workSpace->list = this->list;
}