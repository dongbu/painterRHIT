#include "commandinterpreter.h"
#include <opencv2/opencv.hpp>
#include "math.h"
#include <QFile>
#include <QXmlStreamWriter>
#include "windows.h"
#include "stdio.h"
#include <iostream>

CommandInterpreter::CommandInterpreter(QString projectName)
{
    this->projectName=projectName;
    //painter work//
    picasso = new Painter();
    picasso->setWindowTitle("Painter");
    stopped = true;
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(sendCommand()));

    startPos = 0;
    currentPos = 0;

	//temporary robot work//
	prevContinuous = false;
	connected = false;

	//temporary robot work//

	//temporary write things//
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("Debugging Window:\n");
	//temporary write things//
}

/**
 * @brief will use a loop to start at the current command and continuously add to the painter
 * unless told to stop or reaches the end.
 * @param widget
 */
void CommandInterpreter::beginPaintingCommands(QListWidget* widget, int index){
    startPos = index;
    picasso->show();

    if(stopped){
        CommandInterpreter::buildPointVectors(widget);
		if (connected){
			emit tell_go_home(1);
		}
        stopped = false;
    }
    if(!updateTimer.isActive()){
        updateTimer.start(100);
    }
    picasso->raise();


}

void CommandInterpreter::sendCommand(){
    if(currentPos < x1.size()){
		
		if (connected){
			bool continuous = false;
			if (currentPos < (x1.size() - 1)){
				if (x1.at(currentPos + 1) == x2.at(currentPos) && y1.at(currentPos + 1) == y2.at(currentPos)){
					continuous = true;
				}
				printf("x1: %d\n",x1.at(currentPos + 1));
				printf("x2: %d\n", x2.at(currentPos));
				printf("y1: %d\n", y1.at(currentPos + 1));
				printf("y2: %d\n", y2.at(currentPos));
				printf("if x1 and x2 match as well as y1 and y2, then continuous should be true\n");

				if (continuous){
					printf("continuous");
				}
				else{
					printf("not continuous");
				}

			}
			emit send_Pos(x1.at(currentPos), y1.at(currentPos), x2.at(currentPos), y2.at(currentPos), continuous, prevContinuous, currentPos);
			
			prevContinuous = continuous;
		}
		else{
			picasso->paintCommand(x1.at(currentPos), y1.at(currentPos), x2.at(currentPos), y2.at(currentPos), colorList.at(currentPos), styleList.at(currentPos));
		}
		currentPos++;
			
    }else{
		if (connected){
			emit tell_go_home(0);
		}
        updateTimer.stop();
        currentPos = x1.size();
        stopped = true;
    }
}


/**
 * @brief stops the painting animation.
 */
void CommandInterpreter::stopPaintingCommands(){
    stopped=true;
    updateTimer.stop();
    currentPos = 0;
    picasso->raise();
    picasso->clearPainter();
	if (connected){
		emit(tell_go_home(0));
	}
}

/**
 * @brief pauses the painting animation.
 */
void CommandInterpreter::pausePaintingCommands(){
    if(updateTimer.isActive()){
        updateTimer.stop();
    }
}

/**
 * @brief step forward and paint one command.
 * @param widget
 */
void CommandInterpreter::stepForwardCommands(){
    CommandInterpreter::pausePaintingCommands();
    picasso->raise();
    if(currentPos < x1.size()){
        
		if (connected){
			if (stopped){emit tell_go_home(1);}
			emit send_Pos(x1.at(currentPos), y1.at(currentPos), x2.at(currentPos), y2.at(currentPos), false, false, currentPos);
		}
		else{
			picasso->paintCommand(x1.at(currentPos), y1.at(currentPos), x2.at(currentPos), y2.at(currentPos), colorList.at(currentPos), styleList.at(currentPos));
		}
		stopped = false;
        currentPos++;
    }
}

/**
 * @brief step back and undo painting one command.  Since there is no "unpaint" function,
 * removes all painted commands and then paints up until the previous command.
 * @param widget
 */
void CommandInterpreter::stepBackwardCommands(){
    CommandInterpreter::pausePaintingCommands();
    stopped = false;
    picasso->raise();
    if(currentPos > startPos){
        currentPos--;
        CommandInterpreter::drawUntilCommand(currentPos);
    }
}


/**
 * @brief draws all of the commands up until the given index.
 * @param widget
 * @param stopPos
 */
void CommandInterpreter::drawUntilCommand(int stopPos){
    picasso->clearPainter();
    for(int i = startPos; i < stopPos; i++){
        picasso->paintCommand(x1.at(i),y1.at(i),x2.at(i),y2.at(i),colorList.at(i),styleList.at(i));
    }
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
 * @brief looks at the widget and pulls forth all necessary information from xml files
 * @param widget
 */
void CommandInterpreter::buildPointVectors(QListWidget* widget){
    if(projectName.isEmpty() || projectName.isNull()){
        return;
    }
    x1.clear();
    x2.clear();
    y1.clear();
    y2.clear();
    colorList.clear();
    styleList.clear();



    QList<QListWidgetItem *> listOfCommands = widget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    //used to read each file.
    for(int i = 0; i < listOfCommands.length(); i++){
        addPointsFromFile(listOfCommands.at(i)->text());
    }
}

void CommandInterpreter::addPointsFromFile(QString fileName){
    bool firstPoint = true;
    int prevX;
    int prevY;
    QString currentColor;
    QString currentStyle;

    QFile loadFile;
    loadFile.setFileName(QString("ProjectFiles/") + projectName+ QString("/") + fileName+ QString(".xml"));
    loadFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&loadFile);



    //keep going until the document is finished.
    while(!reader.isEndDocument()){
        reader.readNext();
        if(reader.isStartElement()){

            if(reader.name().toString() == "Line"){
                int j = 0;

                //set the color and linestyle info.
                foreach(const QXmlStreamAttribute &attr, reader.attributes()){
                    if(j == 0){
                        currentColor = attr.value().toString();
                    }else{
                        currentStyle = attr.value().toString();
                    }

                    j++;
                }
            }
            if(reader.name().toString() == "Point"){
                int k = 0;
                foreach(const QXmlStreamAttribute &attr, reader.attributes()){
                    if(firstPoint){
                        if(k == 0){
                            prevX = attr.value().toInt();
                        }else{
                            prevY = attr.value().toInt();
                            firstPoint = false;
                        }
                    }else{
                        if(k == 0){
                            x1.push_back(prevX);
                            x2.push_back(attr.value().toInt());
                            prevX = attr.value().toInt();
                        }else{
                            y1.push_back(prevY);
                            y2.push_back(attr.value().toInt());
                            prevY = attr.value().toInt();
                            colorList.push_back(currentColor);
                            styleList.push_back(currentStyle);
                        }

                    }

                    k++;
                }


            } else if(reader.name().toString() == "FileMalformed"){
                if(reader.attributes().value(0).toString() == "1"){
                    std::cout << "FILE WAS MALFORMED!" << std::endl;
                    return;
                    //potentially highlight poorly made files.
                }
            }
        }

    }
    if(reader.hasError()){
        std::cout << "there was an error in reading the file" <<std::endl;
        std::cout << reader.errorString().toStdString() << std::endl;
        //shouldn't be an issue, but put in just in case.
    }

    loadFile.close();
}

/**
 * @brief clears the painter
 */
void CommandInterpreter::clear(){
    picasso->clearPainter();
}

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

void CommandInterpreter::getInstructed(int current){
	picasso->paintCommand(x1.at(current), y1.at(current), x2.at(current), y2.at(current), colorList.at(current), styleList.at(current));
}

