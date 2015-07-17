
#include "CommandWindow.h"

///Public methods below here
CommandWindow::CommandWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow)
{
    ui->setupUi(this);
	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
	connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommandClicked()));
	connect(ui->actionBackward, SIGNAL(triggered()), this, SLOT(backwardClicked()));
	connect(ui->actionForward, SIGNAL(triggered()), this, SLOT(forwardClicked()));
	connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseClicked()));
	connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(runClicked()));
	connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stopClicked()));
}

CommandWindow::~CommandWindow()
{
    delete ui;
}

void CommandWindow::setShapes(Shapes shapes) {
	this->shapes = shapes;
	populate();
}

void CommandWindow::setSimWindow(DrawWindow *sim){
	this->simWin = sim;
}

///Private methods below herelaunchCommandWindow

void CommandWindow::launchEditorWin(int index) {
	delete currentEditor;
	currentEditor = new EditorWindow();
	currentEditor->setShapeToEdit(*this->shapes.at(index));

}
void CommandWindow::runFrom(int index) {

}
void CommandWindow::runOnly(int index) {

}
void CommandWindow::setBreakPoint(int index) {

}

///Slots below here///
void CommandWindow::addCommand() {
	populate();
}

void CommandWindow::moveUpClicked() {
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex > 0){ 
		shapes.swap(currentIndex, currentIndex - 1);
		populate();
	}
}
void CommandWindow::moveDownClicked() {
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex < ui->listWidget->count()){ 
		shapes.swap(currentIndex, currentIndex + 1);
		populate();
	}

}
void CommandWindow::deleteCommandClicked() {
	int currentIndex = ui->listWidget->currentIndex().row();
	shapes.removeShapeAt(currentIndex);
	populate();
}
void CommandWindow::stopClicked() {
	printf("stop clicked!\n");
	this->shapes.setRunning(0);
}
void CommandWindow::pauseClicked() {
	printf("pause clicked!\n");
	this->shapes.setRunning(0);
}
void CommandWindow::forwardClicked() {
	printf("forward clicked!\n");
}
void CommandWindow::backwardClicked() {
	printf("backward clicked!\n");
}
void CommandWindow::launchRightClick() {
	printf("right clicked!\n");
}
void CommandWindow::runClicked() {
	printf("run clicked!\n");
	this->shapes.drawAll(this->simWin);
}

void CommandWindow::populate(){
	ui->listWidget->clear();
	for (int i = 0; i < shapes.length(); i++){
		QString name = QString::fromStdString(shapes.at(i)->type) + QString::number(shapes.at(i)->getID());
		ui->listWidget->addItem(new QListWidgetItem(name));
	}
}