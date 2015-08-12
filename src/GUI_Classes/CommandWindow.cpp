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
	connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(drawingStopped()));
	connect(ui->actionForward, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(ui->actionBackward, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(this->timer, SIGNAL(timeout()), this, SLOT(showTime()));

	populate();
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

	menu->addAction(new QAction("Run from here", ui->tableWidget));
	menu->addAction(new QAction("Run this command", ui->tableWidget));
	menu->addAction(new QAction("Set Breakpoint", ui->tableWidget));
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
	else emit setBreakPoint(ui->tableWidget->currentRow());
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

void CommandWindow::drawingStopped() {
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
