#include "commandinterpreter.h"
#include <opencv2/opencv.hpp>
#include "math.h"
#include "windows.h"
#include "stdio.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <iostream>
#include <stdio.h>
#include <qdebug.h>

/**
 * @brief constructs a command interpreter--a class designed to handle the step-by-step
 * logic required of the commands sent to both the simulator and the robot itself.
 * @param QString
 */
CommandInterpreter::CommandInterpreter()
{
	printf("CommandInterpreter loading\n");
    //painter work//
	picasso = new DrawWindow(1000 , 800 , "Painter");

    stopped = true;
	paused = false;
	finished = false;
	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(SendNext()));

    //robot work//
	prevContinuous = false;
	connected = false;
    //robot work//
}

/**
 * @brief will use a loop to start at the current command and continuously add to the painter
 * unless told to stop or reaches the end.
 * @param widget
 */
void CommandInterpreter::beginPaintingCommands(int startIndex, int finishIndex){
	this->workSpace->commandIndex = startIndex;
	this->workSpace->finishIndex = finishIndex;

	//base cases
	if (!stopped) { return; }
	if (paused) {
		updateTimer.start();
		return;
	}
	if (connected) { emit tell_go_home(1); }

	//variable setting
	stopped = false;

	//simulator initializing
    picasso->show();

	BuildCommands();

	//Animation timer starting
    updateTimer.start(100);

}


/**
 * @brief reads the class variable "list" and internally stores the resulting commands.
 */
void CommandInterpreter::BuildCommands() {
	//Creating Commands
	listOfCommandTypes.clear();

	QList<QListWidgetItem *> CommandNames = workSpace->list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

	for (int i = workSpace->commandIndex; i < workSpace->finishIndex; i++) {
		MakeLine(CommandNames.at(i)->text());
		listOfCommandTypes.push_back(new QString("Line"));
	}
	workSpace->runFromAdjust = workSpace->commandIndex;
	workSpace->finishIndex -= workSpace->commandIndex;
	workSpace->commandIndex = 0;

	//foreach(QListWidgetItem *name, CommandNames) {

	//		MakeLine(name->text());
	//		listOfCommandTypes.push_back(new QString("Line"));

	//	//if (type == LINE_STRING) {
	//	//	MakeLine(name->text());
	//	//listOfCommandTypes.push_back(new QString("Line"));
	//	//} if (type == SOLID_STRING) {
	//	//	MakeSolid(name->text());
	//	//listOfCommandTypes.push_back(new QString("Solid"));
	//}
}

/**
 * @brief steps to the next action in the preset simulation sequence
 */
void CommandInterpreter::SendNext(){
	//checking for breakpoints
	for (int i = 0; i < breakPointList.size(); i++){
		if ((workSpace->commandIndex + workSpace->runFromAdjust) == breakPointList[i]){
			CommandInterpreter::pausePaintingCommands();
			breakPointList[i] = -50;
			workSpace->list->item(workSpace->commandIndex + workSpace->runFromAdjust)->setTextColor(Qt::black);
			return;
		}
	}

	if (CurrentCommandType == "Line") {
		sendLine(); //Continue an old line command
	} else if (CurrentCommandType == "Solid") {
		sendSolid(); //Continue an old solid command
	} else if (workSpace->commandIndex >= workSpace->finishIndex) {
		updateTimer.stop(); //Quit!  We're done
		paused = false;
		stopped = true;
		finished = true;
	} else if (listOfCommandTypes.front() == QString("Line")) {
		CurrentCommandType = *listOfCommandTypes.front();
		sendLine(); //Start a new line command
	} else if (listOfCommandTypes.front() == QString("Solid")) {
		CurrentCommandType = *listOfCommandTypes.front();
		sendSolid(); //Start a new solid command
	}
}

/**
 * @brief creates a "Line" command from the given file name.
 * @param commandName
 */
void CommandInterpreter::MakeLine(QString commandName) {
	//start xml data extraction
	QFile loadFile;
	loadFile.setFileName(QString("ProjectFiles/") + workSpace->projectName + QString("/") + commandName + QString(".xml"));
	loadFile.open(QIODevice::ReadOnly);
	QXmlStreamReader reader(&loadFile);

	while (!reader.isEndDocument()) { //loop xml file
		reader.readNext();

		//handles color, linestyle, and thickness lines
		if (reader.name().toString() == "Line"){
			int j = 0;
			foreach(const QXmlStreamAttribute &attr, reader.attributes()){
				if (j == 0){
					lineColor.push_back(attr.value().toString());
				}
				else if (j == 1){
					lineStyle.push_back(attr.value().toString());
				}
				else if (j == 2) {
					lineWidth.push_back(attr.value().toInt());
				}
				j++;
			} //end line attribute sorting
		} //End line attribute extraction

		//handles point lines
		if (reader.name().toString() == "Point"){
			int k = 0;
			foreach(const QXmlStreamAttribute &attr, reader.attributes()){
				if (k == 0) {x.push_back(attr.value().toInt());}
				if (k == 1) {y.push_back(attr.value().toInt());}
				k++;
			} //end x/y extraction
		} //end point extraction
	} //End xml data extraction
	loadFile.close();

	//coding vectors (so I know when command is over)
	x.push_back(-50);
	y.push_back(-50);
}

/**
 * @brief creates a "Solid" command from the given file name.
 * @param commandName
 */
void CommandInterpreter::MakeSolid(QString commandName) {
	//fill this method out when solids actually exist
}

/**
 * @brief sends a single "Line paint command" to both the painter
 * and the robot (if connected).
 */
void CommandInterpreter::sendLine() {
	//if (connected) { /*Do robot stuff*/ }

	//color setting
	this->setPaintbrush(QColor(lineColor.at(workSpace->lineAttributeIndex)).name());

	//Thickness setting
	picasso->setLineThickness(lineWidth.at(workSpace->lineAttributeIndex));

	//style setting (TODO:  implement more styles than just "solid"
	picasso->setLineType("solid");
	//lineStyle.at(workSpace->lineAttributeIndex);

	//line drawing
	picasso->drawLine(x.at(workSpace->lineIndex), y.at(workSpace->lineIndex),x.at(workSpace->lineIndex + 1), y.at(workSpace->lineIndex + 1));
	picasso->show();

	//updating index
	workSpace->lineIndex++;

	if (x.at(workSpace->lineIndex + 1) == -50) {  //escape sequence detected (change commands)
		workSpace->list->item(workSpace->commandIndex + workSpace->runFromAdjust)->setBackgroundColor(Qt::green);
		workSpace->commandIndex++;
		workSpace->lineAttributeIndex++;
		workSpace->lineIndex+=2;
		CurrentCommandType = "garbage";
	}
}

void CommandInterpreter::setPaintbrush(QString colorHex) {
	std::string hex = colorHex.toStdString();

	int r, g, b;

	std::string sR, sG, sB;
	sR = hex.substr(1, 2);
	sG = hex.substr(3, 2);
	sB = hex.substr(5, 2);

	int r1 = strtol(&sR.at(0), NULL, 16);
	int r2 = strtol(&sR.at(1), NULL, 8);
	int g1 = strtol(&sG.at(0), NULL, 16);
	int g2 = strtol(&sG.at(1), NULL, 8);
	int b1 = strtol(&sB.at(0), NULL, 16);
	int b2 = strtol(&sB.at(1), NULL, 8);

	r = r1 + r2;
	g = g1 + g2;
	b = b1 + b2;

	picasso->setPenColor(r, g, b);
}

/**
 * @brief sends a single "Solid paint command" to both the painter
 * and the robot (if connected).
 */
void CommandInterpreter::sendSolid() {
	//fill out this method when solids are actally created
}


/**
 * @brief stops the painting animation.  Operates as a quasi-reset.
 */
void CommandInterpreter::stopPaintingCommands(){
	//Index reseting
	ResetIndicies();

	//Class variable resets
    stopped=true;
	paused = false;
    updateTimer.stop();
	listOfCommandTypes.clear();
	CurrentCommandType = "garbage";

	//Line variable resets
	x.clear();
	y.clear();
	lineWidth.clear();
	lineColor.clear();
	lineStyle.clear();

	//Solid variable resets

	//Other class resets
	this->clear();
	if (connected){ emit(tell_go_home(0)); }
}

/**
 * @brief resets all command indicies to 0, recolors CommandList
 */
void CommandInterpreter::ResetIndicies() {
	for (int i = 0; i < (listOfCommandTypes.size() + workSpace->runFromAdjust); i++) {
		workSpace->list->item(i)->setBackgroundColor(Qt::white);
	}

	workSpace->runFromAdjust = 0;
	workSpace->commandIndex = 0;
	workSpace->lineIndex = 0;
	workSpace->lineAttributeIndex = 0;
	workSpace->solidIndex = 0;
}

/**
 * @brief pauses the painting animation.
 */
void CommandInterpreter::pausePaintingCommands(){
	updateTimer.stop();
	paused = true;
}

/**
 * @brief step forward and paint one command.
 * @param widget
 */
void CommandInterpreter::stepForwardCommands(int finishIndex){
	if (stopped){
		picasso->show();
		workSpace->finishIndex = finishIndex;
		this->BuildCommands();
		stopped = false;
	}
	CommandInterpreter::pausePaintingCommands();
	CommandInterpreter::SendNext();
	
}

/**
 * @brief steps back one command (by clearing & repainting all prior)
 * @param widget
 */
void CommandInterpreter::stepBackwardCommands(){
    CommandInterpreter::pausePaintingCommands();

	if (workSpace->commandIndex <= 0) {
		stopPaintingCommands();
		return;
	}

	if (finished) {
		CommandInterpreter::drawUntilCommand(workSpace->commandIndex - 2);
		finished = false;
	} else {
		CommandInterpreter::drawUntilCommand(workSpace->commandIndex - 1);
	}
}


/**
 * @brief draws all of the commands up until the given index.
 * @param widget
 * @param stopPos
 */
void CommandInterpreter::drawUntilCommand(int stopIndex){
	this->clear();
	ResetIndicies();
	while (workSpace->commandIndex <= stopIndex) CommandInterpreter::SendNext();

	workSpace->commandIndex--;
}

/**
 * @brief clears the painter
 */
void CommandInterpreter::clear(){
    picasso->clearWindow(255,255,255);
}

/**
 * @brief Attempts to connect to the specified robot.
 * @param robot
 */
void CommandInterpreter::beginConnecting(QString robot){
    if(robot == "cyton"){
        bender = new CytonController();
        connected = bender->connect();
		connect(this, SIGNAL(tell_go_home(int)), bender, SLOT(go_home(int)));
        connect(bender,SIGNAL(finishedLine(int)),this,SLOT(getInstructed(int)));
        connect(this,SIGNAL(send_Pos(double,double,double,double,bool,bool,int)),bender,SLOT(traceLine(double, double, double, double, bool, bool, int)));
		
    }else if(robot == "ABB"){
        printf("connect to ABB here");
    }
}

void CommandInterpreter::setWorkSpace(WorkSpace *workSpace){
	this->workSpace = workSpace;
	this->stopped = true;
}
