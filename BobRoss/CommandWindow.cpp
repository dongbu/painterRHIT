
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
}

CommandWindow::~CommandWindow()
{
    delete ui;
}

void CommandWindow::setShapes(Shapes shapes) {
	this->shapes = shapes;
	printf("number of shapes: %i\n", shapes.length());
	populate();
	printf("number of shapes: %i\n", this->shapes.length());
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
	printf("number of s: %i\n", shapes.length());

	printf("moving up clicked\n");
	int currentIndex = ui->listWidget->currentIndex().row();
	printf("current index:  %i\n", currentIndex);
	if (currentIndex > 0){ 
		printf("current index is above zero\n");
		printf("swapping %i with %i\n", currentIndex, currentIndex - 1);
		shapes.swap(currentIndex, currentIndex - 1);
		printf("finished swapping\n");
		printf("number of shapes: %i\n", shapes.length());
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
	printf("number of shapes: %i\n", this->shapes.length());

	ui->listWidget->clear();
	for (int i = 0; i < shapes.length(); i++){
		QString name = QString::number(i) + QString::fromStdString(": " + shapes.at(i)->type);
		printf("name: %s\n", name.toStdString().c_str());
		ui->listWidget->addItem(new QListWidgetItem(name));
	}
}