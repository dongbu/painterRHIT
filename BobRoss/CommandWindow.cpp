
#include "CommandWindow.h"

///Public methods below here
CommandWindow::CommandWindow(Shapes *ss, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::CommandWindow)
{
	ui->setupUi(this);
	shapes = ss;

	ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(launchRightClick(QPoint)));
	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
	connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommandClicked()));

	populate();
}

CommandWindow::~CommandWindow()
{
    delete ui;
}

///Slots below here///

void CommandWindow::moveUpClicked() {
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex > 0){ 
		shapes->swap(currentIndex, currentIndex - 1);
		populate();
	}
}
void CommandWindow::moveDownClicked() {
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex < ui->listWidget->count()){ 
		shapes->swap(currentIndex, currentIndex + 1);
		populate();
	}

}
void CommandWindow::deleteCommandClicked() {
	int currentIndex = ui->listWidget->currentIndex().row();
	shapes->removeShapeAt(currentIndex);
	populate();
	emit modifiedCommand();
}

void CommandWindow::populate(){
	ui->listWidget->clear();
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::fromStdString(shapes->at(i)->type) + QString::number(shapes->at(i)->getID());
		ui->listWidget->addItem(new QListWidgetItem(name));
	}
}
void CommandWindow::launchRightClick(QPoint pos) {
	QMenu *menu = new QMenu(ui->listWidget);

	menu->addAction(new QAction("Run from here", ui->listWidget));
	menu->addAction(new QAction("Run this command", ui->listWidget));
	menu->addAction(new QAction("Set Breakpoint", ui->listWidget));
	menu->popup(ui->listWidget->viewport()->mapToGlobal(pos));

	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSort(QAction*)));
}
void CommandWindow::menuSort(QAction *a) {
	if (a->text() == "Run from here") emit runFrom(ui->listWidget->currentRow());
	else if (a->text() == "Run this command") emit runOnly(ui->listWidget->currentRow());
	else emit setBreakPoint(ui->listWidget->currentRow());
}