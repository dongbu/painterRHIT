#include "CommandWindow.h"

/**
 * @brief constructor
 * @param ss
 * @param parent
 */
CommandWindow::CommandWindow(Shapes *ss, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::CommandWindow)
{
	ui->setupUi(this);
	QRect r = QApplication::desktop()->availableGeometry();
	this->move(0, r.bottom() - height() - 50);

	shapes = ss;

	timer = new QTimer(this);
	commandsFinished = 0;
	secondCount = 0;
	minuteCount = 0;
	minPrep = false;

	ui->tableWidget->setColumnCount(3);
	ui->tableWidget->setColumnWidth(0, 35);  //color collumn
	ui->tableWidget->setColumnWidth(1, 50);  //breakpoint collumn
	ui->tableWidget->setColumnWidth(2, 130); //command type collumn

	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(launchRightClick(QPoint)));
	
	connect(ui->tableWidget, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(cellChange(int, int, int, int)));

	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
	connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommandClicked()));

	connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(drawingStarted()));
	connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(drawingCleared()));
	connect(ui->actionForward, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(ui->actionBackward, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(this->timer, SIGNAL(timeout()), this, SLOT(showTime()));

	//building color changing dialog
	colorForm = new QDialog();
	colorUi.setupUi(colorForm);
	colorUi.tableWidget->setRowCount(0);
	colorUi.tableWidget->setColumnCount(3);
	colorUi.tableWidget->insertRow(ui->tableWidget->rowCount());
	colorUi.tableWidget->setColumnWidth(0, 35); //color collumn
	colorUi.tableWidget->setColumnWidth(1, 50); //breakpoint collumn
	colorUi.tableWidget->setColumnWidth(2, 130); //command type collumn
	colorUi.ChangeAll->setChecked(true);
	connect(colorUi.ConfirmButton, SIGNAL(pressed()), this, SLOT(colorChangeConfirmed()));

	populate();
	this->hide();
}
/**
 * @brief destructor
 */
CommandWindow::~CommandWindow()
{
	delete ui;
}

/**
 * @brief move command up in window (and vec)
 */
void CommandWindow::moveUpClicked() {
	int currentIndex = ui->tableWidget->currentRow();
	if (currentIndex <= 0) return;
	if (currentIndex > 0){
		shapes->swap(currentIndex, currentIndex - 1);
		populate();
	}
	ui->tableWidget->setCurrentCell(currentIndex - 1, 2);
}
/**
 * @brief move command down in window (and vec)
 */
void CommandWindow::moveDownClicked() {
	if (ui->tableWidget->rowCount() <= 0 || ui->tableWidget->currentRow() == ui->tableWidget->rowCount() - 1) return;

	int currentIndex = ui->tableWidget->currentRow();
	if (currentIndex < ui->tableWidget->rowCount()){
		shapes->swap(ui->tableWidget->currentRow(), ui->tableWidget->currentRow() + 1);
		populate();
	}
	ui->tableWidget->setCurrentCell(currentIndex + 1, 1);

}
/**
 * @brief remove command from window (and vec)
 */
void CommandWindow::deleteCommandClicked() {
	if (ui->tableWidget->rowCount() <= 0) return;
	int currentIndex = ui->tableWidget->currentRow();
	shapes->removeShapeAt(currentIndex);
	populate();
	emit modifiedCommand();
}

/**
 * @brief populate list with commands.
 */
void CommandWindow::populate(){
	//reseting
	this->show();
	ui->tableWidget->clear();
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("rgb"));
	ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("break"));
	ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("command type"));

	//refilling the list with shape data
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::fromStdString(shapes->at(i)->type);

		//cell creation
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());
		ui->tableWidget->setItem(i, 0, new QTableWidgetItem()); //rgb
		ui->tableWidget->setItem(i, 2, new QTableWidgetItem(name)); //command type

		//color setting
		cv::Scalar penCol = shapes->at(i)->getPenColor();
		ui->tableWidget->item(i, 0)->setBackgroundColor(QColor(penCol[0], penCol[1], penCol[2]));
		if (shapes->at(i)->isBreakPoint){ recieveRunColor(i, "red"); }
		else { ui->tableWidget->setItem(i, 1, new QTableWidgetItem("no")); }

		//centering
		ui->tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter); // breakpoint
		ui->tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter); // command type
	}

	//reseting
	ui->CommandsRun->setText(QString::number(commandsFinished) + QString("/") + QString::number(shapes->length()) + QString(" run"));
	ui->CommandsRun->setAlignment(Qt::AlignCenter);
}
/**
 * @brief recieve right click
 * @param pos
 */
void CommandWindow::launchRightClick(QPoint pos) {
	QMenu *menu = new QMenu(ui->tableWidget);
	int col = ui->tableWidget->currentColumn();

	if (col == 0) { //color menu
		menu->addAction(new QAction("Change Color", ui->tableWidget));
	} else if (col == 1) { //breakpoint menu
		menu->addAction(new QAction("Set Breakpoint", ui->tableWidget));
	} else if (col == 2) { //run menu
		menu->addAction(new QAction("Run from here", ui->tableWidget));
		menu->addAction(new QAction("Run this command", ui->tableWidget));
	}

	menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));

	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSort(QAction*)));
}
/**
 * @brief sort menu from right click.
 * @param a
 */
void CommandWindow::menuSort(QAction *a) {
	if (a->text() == "Run from here") emit runFrom(ui->tableWidget->currentRow());
	else if (a->text() == "Run this command") emit runOnly(ui->tableWidget->currentRow());
	else if (a->text() == "Set Breakpoint") emit setBreakPoint(ui->tableWidget->currentRow());
	else if (a->text() == "Change Color") {
		int curRow = ui->tableWidget->currentRow();
		cv::Scalar penCol = shapes->at(curRow)->getPenColor();
		QTableWidgetItem *rgb = new QTableWidgetItem();
		rgb->setBackgroundColor(QColor(penCol[0], penCol[1], penCol[2]));
		colorUi.tableWidget->setItem(0, 0, rgb); //rgb
		colorUi.comboBox->setCurrentText("current color");
		if (shapes->at(curRow)->isBreakPoint) {
		colorUi.tableWidget->setItem(0, 1, new QTableWidgetItem("yes"));
		} else {
			colorUi.tableWidget->setItem(0, 1, new QTableWidgetItem("no"));
		}
		colorUi.tableWidget->item(0, 1)->setTextAlignment(Qt::AlignCenter);
		colorUi.tableWidget->setItem(0, 2, new QTableWidgetItem(*ui->tableWidget->item(curRow, 2)));

		colorForm->show();
	}
}

/**
 * @brief toggle identifier for already run commands.
 * @param index
 * @param runToggle
 */
void CommandWindow::recieveRunColor(int index, QString runToggle){
	if (index >= ui->tableWidget->rowCount()){
		index = ui->tableWidget->rowCount() - 1;
	}
	if (runToggle == "green"){
		ui->tableWidget->item(index, 2)->setBackgroundColor("green");
		if (index == shapes->length() - 1) { this->timer->stop(); }
		if (this->commandsFinished >= shapes->length()) { return; }
		this->commandsFinished++;
		ui->CommandsRun->setText(QString::number(commandsFinished) + QString("/") + QString::number(shapes->length()) + QString(" run"));
		ui->CommandsRun->setAlignment(Qt::AlignCenter);
	}
	else if (runToggle == "yellow") {
		ui->tableWidget->item(index, 2)->setBackgroundColor("yellow");
		ui->tableWidget->scrollToItem(ui->tableWidget->item(index, 0), QAbstractItemView::PositionAtCenter);
		emit highlightShape(index);
	}
	else if (runToggle == "white") {
		ui->tableWidget->item(index, 2)->setBackgroundColor("white");
		this->commandsFinished = 0;
		ui->CommandsRun->setText(QString::number(commandsFinished) + QString("/") + QString::number(shapes->length()) + QString(" run"));
		ui->CommandsRun->setAlignment(Qt::AlignCenter);
	}
	else if (runToggle == "red") {
		ui->tableWidget->setItem(index, 1, new QTableWidgetItem("yes"));
		ui->tableWidget->item(index, 1)->setTextAlignment(Qt::AlignCenter);
	}
}

/**
 * @brief clear all "already run" commands.
 */
void CommandWindow::recieveClearRunColors(){
	for (int i = 0; i < ui->tableWidget->rowCount(); i++){
		recieveRunColor(i, "white");
	}
}

void CommandWindow::drawingStarted() {
	if (shapes->length() == 0) { return; }
	timer->start();
	startTime = QTime::currentTime();
}

void CommandWindow::drawingPaused() {
	if (!this->timer->isActive()) {
		return;
	}

	this->timer->stop();
	secondCount = QTime::currentTime().second() - startTime.second() + secondCount;
	if (secondCount < 0) { secondCount = 60 + secondCount; }
}

void CommandWindow::drawingCleared() {
	this->timer->stop();
	ui->TimeEllapsed->setText(QString::number(0) + QString(":") + QString::number(0) + QString(" ellapsed"));
	ui->TimeEllapsed->setAlignment(Qt::AlignCenter);
	secondCount = 0;
	minuteCount = 0;
	minPrep = false;
}

void CommandWindow::showTime() {
	int sec = QTime::currentTime().second() - startTime.second() + secondCount;
	if (sec < 0) { sec = 60 + sec; }

	//updating minute logic
	if (sec == 1) { minPrep = true; }
	if (sec == 0 && minPrep) {
		minPrep = false;
		minuteCount++;
	}

	ui->TimeEllapsed->setText(QString::number(minuteCount) + QString(":") + QString::number(sec) + QString(" ellapsed"));
	ui->TimeEllapsed->setAlignment(Qt::AlignCenter);
}

void CommandWindow::cellChange(int curRow, int curCol, int prevRow, int prevCol) { emit highlightShape(curRow);}

void CommandWindow::colorChangeConfirmed() {
	colorForm->hide();
	std::vector<int> NewRGB = getColor(colorUi.comboBox->currentText());
	cv::Scalar OldRGB = shapes->at(ui->tableWidget->currentRow())->getPenColor();
	if (NewRGB[0] == -1) return; //no change detected.

	if (colorUi.ChangeAll->isChecked()) {
		for (int i = 0; i < shapes->length(); i++) {
			cv::Scalar checkRGB = shapes->at(i)->getPenColor();
			if (checkRGB[0] == OldRGB[0] && checkRGB[1] == OldRGB[1] && checkRGB[2] == OldRGB[2]) {
				shapes->at(i)->setPenColor(NewRGB[0], NewRGB[1], NewRGB[2]);
			}
		}
	} else {
		shapes->at(ui->tableWidget->currentRow())->setPenColor(NewRGB[0], NewRGB[1], NewRGB[2]);
	}

	populate();
}

/**
* @brief transforms a text color into useable rgb code.
*/
std::vector<int> CommandWindow::getColor(QString col) {
	std::vector<int> toReturn;
	if (col == "black") {
		toReturn.push_back(0);
		toReturn.push_back(0);
		toReturn.push_back(0);
	}
	else if (col == "orange") {
		toReturn.push_back(30);
		toReturn.push_back(144);
		toReturn.push_back(255);
	}
	else if (col == "yellow") {
		toReturn.push_back(0);
		toReturn.push_back(255);
		toReturn.push_back(255);
	}
	else if (col == "green") {
		toReturn.push_back(34);
		toReturn.push_back(139);
		toReturn.push_back(34);
	}
	else if (col == "red") {
		toReturn.push_back(34);
		toReturn.push_back(34);
		toReturn.push_back(178);
	}
	else if (col == "blue") {
		toReturn.push_back(255);
		toReturn.push_back(144);
		toReturn.push_back(30);
	}
	else if (col == "purple") {
		toReturn.push_back(240);
		toReturn.push_back(32);
		toReturn.push_back(160);
	}
	else {
		toReturn.push_back(-1);
		toReturn.push_back(-1);
		toReturn.push_back(-1);
	}

	return toReturn;
}