
#include "CommandWindow.h"

///Public methods below here
CommandWindow::CommandWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow)
{
    ui->setupUi(this);
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

}
void CommandWindow::moveDownClicked() {

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
	for (int i = 0; i < shapes->length(); i++){

	}
}