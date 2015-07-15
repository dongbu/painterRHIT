#include "painter.h"
#include <qstring.h>
#include "commandinterpreter.h"

painter::painter(){
	printf("WorkSpace loading\n");
	projectName = "";
	projectLocation = "ProjectFiles/Temp";
	lineIndex = 0;
	commandIndex = 0;
	solidIndex = 0;
	lineAttributeIndex = 0;
	runFromAdjust = 0;
	finishIndex = 0;
	//commandStack = ???
	canvasSize = new QSize(3, 4);
	pixelToCm = 0;
	fillMethod = "random";
}

void painter::AddToList(Line toAdd){
	printf("not implemented yet\n");
}
void painter::LaunchSim(){
	mainWin.commandView->show();
	mainWin.commandView->RunFromStart_triggered();
}
void painter::setDimensions(int height, int width){
	canvasSize = new QSize(width, height);
	mainWin.drawOn->setFixedSize(*canvasSize);
	if (height > 80){
		mainWin.setFixedHeight(height + 50);
	}
	else{
		mainWin.setFixedHeight(130);
	}
	if (width > 120){
		mainWin.setFixedWidth(width + 50);
	}
	else{
		mainWin.setFixedWidth(170);
	}
	mainWin.drawOn2->setFixedSize(*canvasSize);
	//mainWin.commandView->interpreter->picasso->clearWindow();
	//mainWin.commandView->interpreter = new CommandInterpreter(width,height);

	

}

void painter::launchRobot(){
	printf("not implemented yet\n");
}
void painter::setGuiEditable(bool state){
	if (state){
		mainWin.show();
		mainWin.commandView->show();
	}
	else{
		mainWin.hide();
		mainWin.commandView->hide();
	}
}
void painter::save(){
	mainWin.on_actionSave_triggered();
	this->projectLocation = mainWin.projectLocation;
	this->projectName = mainWin.projectName;
	
}
void painter::load(){
	mainWin.on_actionOpen_triggered();
	this->projectLocation = mainWin.projectLocation;
	this->projectName = mainWin.projectName;
}

void painter::newProject(){
	mainWin.on_actionNew_triggered();
	this->projectLocation = mainWin.projectLocation;
	this->projectName = mainWin.projectName;
}

void painter::addLine(Line *toAdd){
	projectLocation = mainWin.projectLocation;
	projectName = mainWin.projectName;
	//searches through and sets the default name to 1 + the largest.
	QList<QListWidgetItem *> listOfCommands = mainWin.commandView->list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
	QList<QString> texts;
	foreach(QListWidgetItem *item, listOfCommands){
		texts.append(item->text());
	}
	int k = 1;
	QString currentName = QString("PointMap_") + QString::number(k);
	while (texts.contains(currentName)){
		k++;
		currentName = QString("PointMap_") + QString::number(k);
	}

	toAdd->setName(currentName);
	mainWin.commandView->list->setCurrentRow(mainWin.commandView->list->count());
	toAdd->Add_Command_Clicked(projectLocation, mainWin.commandView->list);
	GuiLoadSave::writeCommandListToFolder(projectLocation, mainWin.commandView->list);
}