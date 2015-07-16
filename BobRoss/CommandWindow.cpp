
#include "CommandWindow.h"

///Public methods below here
CommandWindow::CommandWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow)
{
    ui->setupUi(this);
	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
}

CommandWindow::~CommandWindow()
{
    delete ui;
}

void CommandWindow::setShapes(Shapes *shapes) {
	this->shapes = shapes;
	populate();
}

///Private methods below here

void CommandWindow::launchEditorWin(int index) {
	delete currentEditor;
	currentEditor = new EditorWindow();
	currentEditor->setShapeToEdit(*this->shapes->at(index));

}
void CommandWindow::runFrom(int index) {

}
void CommandWindow::runOnly(int index) {

}
void CommandWindow::setBreakPoint(int index) {

}

///Slots below here///
void CommandWindow::addCommand(Shape s) {
	this->shapes->addShape(&s);
	populate();
}

void CommandWindow::moveUpClicked() {
	printf("moving up\n");
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex > 0){ 
		shapes->swap(currentIndex, currentIndex - 1);
		populate();
	}
}
void CommandWindow::moveDownClicked() {
	printf("moving down\n");
	int currentIndex = ui->listWidget->currentIndex().row();
	if (currentIndex < ui->listWidget->count()){ 
		shapes->swap(currentIndex, currentIndex + 1);
		populate();
	}

}
void CommandWindow::deleteCommandClicked() {

}
void CommandWindow::stopClicked() {

}
void CommandWindow::pauseClicked() {

}
void CommandWindow::forwardClicked() {

}
void CommandWindow::backwardClicked() {

}
void CommandWindow::launchRightClick() {

}
void CommandWindow::runClicked() {

}

void CommandWindow::populate(){
	printf("populating\n");
	ui->listWidget->clear();
	printf("entering loop\n");
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::number(i) + QString::fromStdString(": " + shapes->at(i)->type);
		printf("name: %s\n", name.toStdString().c_str());
		ui->listWidget->addItem(new QListWidgetItem(name));
	}
}