
#include "CommandWindow.h"

///Public methods below here
CommandWindow::CommandWindow(Shapes *ss, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow)
{
    ui->setupUi(this);
	shapes = ss;

	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
	connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommandClicked()));
	connect(ui->actionBackward, SIGNAL(triggered()), this, SLOT(backwardClicked()));
	connect(ui->actionForward, SIGNAL(triggered()), this, SLOT(forwardClicked()));
	connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseClicked()));
	connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(runClicked()));
	connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stopClicked()));

	populate();
}

CommandWindow::~CommandWindow()
{
    delete ui;
}

void CommandWindow::setSimWindow(DrawWindow *sim){
	this->simWin = sim;
}

///Private methods below herelaunchCommandWindow
void CommandWindow::runFrom(int index) {
	printf("TODO: implement runFrom\n");
}
void CommandWindow::runOnly(int index) {
	printf("TODO: implement runOnly\n");
}
void CommandWindow::setBreakPoint(int index) {
	printf("TODO: implement setBreakPoint\n");
}

///Slots below here///
void CommandWindow::addCommand() {
	populate();
}

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
void CommandWindow::stopClicked() {
	printf("TODO: reset position and toggles\n");
	this->shapes->setRunning(false);
	this->simWin->clearWindow(255, 255, 255);
}
void CommandWindow::pauseClicked() {
	this->shapes->setRunning(false);
}
void CommandWindow::forwardClicked() {
	printf("TODO: implement forwardClicked\n");
}
void CommandWindow::backwardClicked() {
	printf("TODO: implement backwardClicked\n");
}
void CommandWindow::launchRightClick() {
	printf("TODO: implement launchRightClick\n");
}
void CommandWindow::runClicked() {
	printf("TODO: keep track of position\n");
	printf("TODO: change colors\n");
	printf("TODO: deal with breakpoints and toggles\n");
	this->shapes->drawAll(this->simWin);
}

void CommandWindow::populate(){
	ui->listWidget->clear();
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::fromStdString(shapes->at(i)->type) + QString::number(shapes->at(i)->getID());
		ui->listWidget->addItem(new QListWidgetItem(name));
	}
}