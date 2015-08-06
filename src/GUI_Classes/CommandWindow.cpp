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

	ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(launchRightClick(QPoint)));
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
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex > 0){
		shapes->swap(currentIndex, currentIndex - 1);
		populate();
	}
}
/**
 * @brief move command down in window (and vec)
 */
void CommandWindow::moveDownClicked() {
	if (ui->listWidget->count() <= 0 || ui->listWidget->currentIndex().row() == ui->listWidget->count() - 1) return;

	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex < ui->listWidget->count()){
		shapes->swap(currentIndex, currentIndex + 1);
		populate();
	}

}
/**
 * @brief remove command from window (and vec)
 */
void CommandWindow::deleteCommandClicked() {
	if (ui->listWidget->count() <= 0) return;
	int currentIndex = ui->listWidget->currentIndex().row();
	shapes->removeShapeAt(currentIndex);
	populate();
	emit modifiedCommand();
}

/**
 * @brief populate list with commands.
 */
void CommandWindow::populate(){
	ui->listWidget->clear();
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::fromStdString(shapes->at(i)->type) + QString::number(shapes->at(i)->getID());
		ui->listWidget->addItem(new QListWidgetItem(name));
	}
}
/**
 * @brief recieve right click
 * @param pos
 */
void CommandWindow::launchRightClick(QPoint pos) {
	QMenu *menu = new QMenu(ui->listWidget);

	menu->addAction(new QAction("Run from here", ui->listWidget));
	menu->addAction(new QAction("Run this command", ui->listWidget));
	menu->addAction(new QAction("Set Breakpoint", ui->listWidget));
	menu->popup(ui->listWidget->viewport()->mapToGlobal(pos));

	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSort(QAction*)));
}
/**
 * @brief sort menu from right click.
 * @param a
 */
void CommandWindow::menuSort(QAction *a) {
	if (a->text() == "Run from here") emit runFrom(ui->listWidget->currentRow());
	else if (a->text() == "Run this command") emit runOnly(ui->listWidget->currentRow());
	else emit setBreakPoint(ui->listWidget->currentRow());
}

/**
 * @brief toggle identifier for already run commands.
 * @param index
 * @param runToggle
 */
void CommandWindow::recieveRunColor(int index, QString runToggle){
	if (index >= ui->listWidget->count()){
		index = ui->listWidget->count() - 1;
	}
	if (runToggle == "green"){
		ui->listWidget->item(index)->setBackgroundColor("green");
	}
	else if (runToggle == "yellow") {
		ui->listWidget->item(index)->setBackgroundColor("yellow");
	}
	else if (runToggle == "white") {
		ui->listWidget->item(index)->setBackgroundColor("white");
	}
	else if (runToggle == "red") {
		ui->listWidget->item(index)->setBackgroundColor("red");
	}
}

/**
 * @brief clear all "already run" commands.
 */
void CommandWindow::recieveClearRunColors(){
	for (int i = 0; i < ui->listWidget->count(); i++){
		recieveRunColor(i, false);
	}
}
