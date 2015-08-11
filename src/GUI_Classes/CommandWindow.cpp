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

	ui->tableWidget->setColumnCount(2);
	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(launchRightClick(QPoint)));
	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
	connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommandClicked()));

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
	if (currentIndex < 0 || (currentIndex == 0 && ui->tableWidget->rowCount() == 1)) return;
	if (currentIndex > 0){
		shapes->swap(currentIndex, currentIndex - 1);
		populate();
	}
	ui->tableWidget->setCurrentCell(currentIndex - 1, 0);
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
	ui->tableWidget->setCurrentCell(currentIndex + 1, 0);

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
	ui->tableWidget->clear();
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("command type"));
	ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("command color"));

	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::fromStdString(shapes->at(i)->type);

		//cell creation
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());
		ui->tableWidget->setItem(i, 0, new QTableWidgetItem(name));
		ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
		ui->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);

		//color setting
		cv::Scalar penCol = shapes->at(i)->getPenColor();
		ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(penCol[0], penCol[1], penCol[2]));
		if (shapes->at(i)->isBreakPoint){
			recieveRunColor(i, "red");
		}
	}
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
		ui->tableWidget->item(index, 0)->setBackgroundColor("green");
	}
	else if (runToggle == "yellow") {
		ui->tableWidget->item(index, 0)->setBackgroundColor("yellow");
	}
	else if (runToggle == "white") {
		ui->tableWidget->item(index, 0)->setBackgroundColor("white");
	}
	else if (runToggle == "red") {
		ui->tableWidget->item(index, 0)->setBackgroundColor("red");
	}
}

/**
 * @brief clear all "already run" commands.
 */
void CommandWindow::recieveClearRunColors(){
	for (int i = 0; i < ui->tableWidget->rowCount(); i++){
		recieveRunColor(i, false);
	}
}
