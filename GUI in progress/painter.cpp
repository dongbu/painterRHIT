#include "painter.h"

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

}
void painter::LaunchSim(){
	mainWin.commandView->show();
	mainWin.commandView->RunFromStart_triggered();
}
void painter::setDimensions(int height, int width){
	canvasSize = new QSize(width, height);
}

void painter::launchRobot(){

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
}
void painter::load(std::string path){
	mainWin.on_actionOpen_triggered();
}

void painter::newProject(){
	mainWin.on_actionNew_triggered();
}

void painter::addLine(Line toAdd){

}