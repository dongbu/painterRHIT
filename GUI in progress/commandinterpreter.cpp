#include "commandinterpreter.h"
#include "math.h"
#include <QFile>
#include <QXmlStreamWriter>

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


    //painter work//
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
        stopped = false;
    }
    if(!updateTimer.isActive()){
    updateTimer.start(100);
    }
    picasso->raise();


}

void CommandInterpreter::sendCommand(){
    if(currentPos < x1.size()){
        picasso->paintCommand(x1.at(currentPos),y1.at(currentPos),x2.at(currentPos),y2.at(currentPos),colorList.at(currentPos),styleList.at(currentPos));
        currentPos++;
    }else{
        updateTimer.stop();
        currentPos = 0;
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
        picasso->paintCommand(x1.at(currentPos),y1.at(currentPos),x2.at(currentPos),y2.at(currentPos),colorList.at(currentPos),styleList.at(currentPos));
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
    if(currentPos > startPos){
        picasso->raise();
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

    int prevX;
    int prevY;
    QString currentColor;
    QString currentStyle;

    QList<QListWidgetItem *> listOfCommands = widget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    for(int i = 0; i < listOfCommands.length(); i++){
        bool firstPoint = true;
        QFile loadFile;
        loadFile.setFileName(QString("ProjectFiles/") + projectName+ QString("/") + listOfCommands.at(i)->text()+ QString(".xml"));
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
//                        std::cout<<attr.value().toInt()<<std::endl;
                        std::cout<<"k: " <<k<<std::endl;
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
            //shouldn't be an issue, but put in just in case.
        }

        loadFile.close();
    }
}
