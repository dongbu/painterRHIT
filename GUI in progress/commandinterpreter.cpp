#include "commandinterpreter.h"
#include <opencv2/opencv.hpp>
#include "math.h"
#include <QFile>
#include <QXmlStreamWriter>
#include "windows.h"
#include "stdio.h"
#include <iostream> //hi
#include <qdebug.h>


/**
 * @brief constructs a command interpreter--a class designed to handle the step-by-step
 * logic required of the commands sent to both the simulator and the robot itself.
 * @param QString
 */
CommandInterpreter::CommandInterpreter(QString projectName)
{
    this->projectName=projectName;
    //painter work//
    picasso = new Painter();
    picasso->setWindowTitle("Painter");
    stopped = true;
	paused = false;
	finished = false;
	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(SendNext()));

	ResetIndicies();

    //robot work//
	prevContinuous = false;
	connected = false;
    //robot work//

    //temporary write things in console//
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("Debugging Window:\n");
    //temporary write things in console//
}

/**
 * @brief will use a loop to start at the current command and continuously add to the painter
 * unless told to stop or reaches the end.
 * @param widget
 */
void CommandInterpreter::beginPaintingCommands(int index){
	//base cases
	if (!stopped) { return; }
	if (paused) {
		updateTimer.start();
		return;
	}
	if (connected) { emit tell_go_home(1); }

	//variable setting
    startCommandIndex = index;
	stopped = false;

	//simulator initializing
    picasso->show();
	picasso->raise();

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
	QList<QListWidgetItem *> CommandNames = list->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
	foreach(QListWidgetItem *name, CommandNames) {
		MakeLine(name->text());
		listOfCommandTypes.push_back(new QString("Line"));
		//if (type == LINE_STRING) {
		//	MakeLine(name->text());
		//listOfCommandTypes.push_back(new QString("Line"));
		//} if (type == SOLID_STRING) {
		//	MakeSolid(name->text());
		//listOfCommandTypes.push_back(new QString("Solid"));
		//} if (type == PIXEL_STRING) {
		//	MakePixel(name->text());
		//listOfCommandTypes.push_back(new QString("Pixel"));
		//}
	}
}

/**
 * @brief sets the class variable list
 * @param list
 */
void CommandInterpreter::setList(QListWidget *list){
	this->list = list;
}

/**
 * @brief steps to the next action in the preset simulation sequence
 */
void CommandInterpreter::SendNext(){
	if (CurrentCommandType == "Line") {
		sendLine(); //Continue an old line command
	} else if (CurrentCommandType == "Solid") {
		sendSolid(); //Continue an old solid command
	} else if (CurrentCommandType == "Pixel") {
		sendPixel(); //Continue an old pixel command
	} else if (commandIndex >= listOfCommandTypes.size()) {
		updateTimer.stop(); //Quit!  We're done
		finished = true;
	} else if (listOfCommandTypes.front() == QString("Line")) {
		CurrentCommandType = *listOfCommandTypes.front();
		sendLine(); //Start a new line command
	} else if (listOfCommandTypes.front() == QString("Solid")) {
		CurrentCommandType = *listOfCommandTypes.front();
		sendSolid(); //Start a new solid command
	} else if (listOfCommandTypes.front() == QString("Pixel")) {
		CurrentCommandType = *listOfCommandTypes.front();
		sendPixel(); //Start a new pixel command
	}
}

/**
 * @brief creates a "Line" command from the given file name.
 * @param commandName
 */
void CommandInterpreter::MakeLine(QString commandName) {
	//start xml data extraction
	QFile loadFile;
	loadFile.setFileName(QString("ProjectFiles/") + projectName + QString("/") + commandName + QString(".xml"));
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
 * @brief creates a "Pixel" command from the given file name.
 * @param commandName
 */
void CommandInterpreter::MakePixel(QString commandName) {
	//fill this method out when pixels actually exist
}

/**
 * @brief sends a single "Line paint command" to both the painter
 * and the robot (if connected).
 */
void CommandInterpreter::sendLine() {
	//if (connected) { /*Do robot stuff*/ }
	this->picasso->paintCommand(x.at(lineIndex), y.at(lineIndex), x.at(lineIndex + 1), y.at(lineIndex + 1),
		lineColor.at(lineAttributeIndex), lineStyle.at(lineAttributeIndex), lineWidth.at(lineAttributeIndex));
	lineIndex++;
	if (x.at(lineIndex + 1) == -50) {
		list->item(commandIndex)->setBackgroundColor(Qt::green);
		commandIndex++;
		lineAttributeIndex++;
		lineIndex+=2;
		CurrentCommandType = "garbage";
	}
}

/**
 * @brief sends a single "Solid paint command" to both the painter
 * and the robot (if connected).
 */
void CommandInterpreter::sendSolid() {
	//fill out this method when solids are actally created
}

/**
 * @brief sends a single "Pixel paint command" to both the painter
 * and the robot (if connected).
 */
void CommandInterpreter::sendPixel() {
	//fill out this method when pixels are actually created
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

	//Pixel variable resets

	//Other class resets
    picasso->raise();
    picasso->clearPainter();
	if (connected){ emit(tell_go_home(0)); }
}

/**
 * @brief resets all command indicies to 0, recolors CommandList
 */
void CommandInterpreter::ResetIndicies() {
	for (int i = 0; i < (listOfCommandTypes.count()); i++) {
		list->item(i)->setBackgroundColor(Qt::white);
	}
	
	commandIndex = 0;
	lineIndex = 0;
	lineAttributeIndex = 0;
	solidIndex = 0;
	pixelIndex = 0;
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
void CommandInterpreter::stepForwardCommands(){
	if (stopped){
		picasso->show();
		this->BuildCommands();
	}
	CommandInterpreter::pausePaintingCommands();
    picasso->raise();
	CommandInterpreter::SendNext();
}

/**
 * @brief steps back one command (by clearing & repainting all prior)
 * @param widget
 */
void CommandInterpreter::stepBackwardCommands(){
	picasso->raise();
    CommandInterpreter::pausePaintingCommands();
    stopped = true;

	if (commandIndex <= 0) {
		stopPaintingCommands();
		return;
	}

	if (finished) {
		CommandInterpreter::drawUntilCommand(commandIndex - 2);
		finished = false;
	} else {
		CommandInterpreter::drawUntilCommand(commandIndex - 1);
	}
}


/**
 * @brief draws all of the commands up until the given index.
 * @param widget
 * @param stopPos
 */
void CommandInterpreter::drawUntilCommand(int stopIndex){
    picasso->clearPainter();
	ResetIndicies();
	while (commandIndex <= stopIndex) CommandInterpreter::SendNext();

	commandIndex--;
}

/**
 * @brief sets the name of the project this interpreter is associated with.
 * @param name
 */
void CommandInterpreter::setProjectName(QString name){
    this->projectName=name;
    this->stopped = true;
}

/**
 * @brief clears the painter
 */
void CommandInterpreter::clear(){
    picasso->clearPainter();
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
