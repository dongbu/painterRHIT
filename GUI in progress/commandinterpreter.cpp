#include "commandinterpreter.h"
#include "windows.h"
#include "pairedcoordinates.h"

CommandInterpreter::CommandInterpreter(QString projectName)
{
    this->projectName=projectName;
    //painter work//
    picasso = new Painter();
    picasso->setWindowTitle("Painter");
    //painter work//
}

/**
 * @brief will use a loop to start at the current command and continuously add to the painter
 * unless told to stop or reaches the end.
 * @param widget
 */
void CommandInterpreter::beginPaintingCommands(QListWidget* widget){

}

/**
 * @brief tells the beginPaintingCommands function that it should stop.
 * @param widget
 */
void CommandInterpreter::stopPaintingCommands(QListWidget* widget){

}

/**
 * @brief step forward and paint one command.
 * @param widget
 */
void CommandInterpreter::stepForwardCommands(QListWidget* widget){

}

/**
 * @brief step back and undo painting one command.  Since there is no "unpaint" function,
 * removes all painted commands and then paints up until the previous command.
 * @param widget
 */
void CommandInterpreter::stepBackwardCommands(QListWidget* widget){

}
/**
 * @brief clears the paint space
 */
void CommandInterpreter::clearPainter(){

}

/**
 * @brief draws all of the commands up until the given index.
 * @param widget
 * @param stopPos
 */
void CommandInterpreter::drawUntilCommand(QListWidget* widget, int stopPos){

}

/**
 * @brief gets a list of the coordinates for a given command.
 * @param commandName
 * @return
 */
QList<PairedCoordinates> CommandInterpreter::getCommandCoords(QString commandName){
    return QList<PairedCoordinates>();
}

/**
 * @brief sets the name of the project this interpreter is associated with.
 * @param name
 */
void CommandInterpreter::setProjectName(QString name){
    this->projectName=name;
}
