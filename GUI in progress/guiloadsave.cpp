#include "guiloadsave.h"
#include "mainwindow.h"

GuiLoadSave::GuiLoadSave()
{

}


//written in this horrible fashion to ensure compiling.  Will need updated with the new code upgrade.
boolean GuiLoadSave::writeCommandToFolder(QString ProjectName, QWidget *CommandEditor, QListWidget *CommandList){

    //boolean checker to make sure the file inputs are correct.
    bool fileMalformed = false;

    //gets all the line edit pieces and puts them into an array
    QList<QLineEdit *> lineEdits = CommandEditor->findChildren<QLineEdit *>();

    //gets all the combobox pieces and puts them into an array
    QList<QComboBox *> comboBoxes = CommandEditor->findChildren<QComboBox *>();

    //the second object is the filename.
    QString fileName = lineEdits.at(2)->text();

    //adds file stuff to the commandlist.
    CommandList->addItem(fileName);

    //sets up a save file to put the information into.  Should overwrite any previous file with same name in directory.
    QString fileLoc = QString("ProjectFiles/") + ProjectName + QString("/") + fileName + QString(".xml");
    QFile saveFile;
    saveFile.setFileName(fileLoc);
    saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QXmlStreamWriter writer(&saveFile);

    //set up the xml doc
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeComment(fileName);

    //begin writing
    writer.writeStartElement("Command");

    //deals with line info
    writer.writeComment("Line Information");
    writer.writeStartElement("Line");
    writer.writeAttribute("color",comboBoxes.at(0)->property("currentText").toString());
    writer.writeAttribute("style", comboBoxes.at(1)->property("currentText").toString());
    writer.writeEndElement();//Line

    //deals with pointmap
    writer.writeComment("Point Map Information");
    writer.writeStartElement("PointMap");
    writer.writeAttribute("length",QString::number(lineEdits.count()-1));
    for(int i = 0; i < lineEdits.count(); i++){
        if(i == 2){
            // do nothing.
            //the second one is the command name since it was the third thing added.  sorry for any confusion.
        }else{
            writer.writeStartElement("Point");
            QString point = lineEdits.at(i)->text();
            QStringList xy = point.split(',');
            QString xValue = xy.value(0);
            QString yValue = xy.value(1);
            writer.writeAttribute("x",xValue);
            writer.writeAttribute("y",yValue);
            writer.writeEndElement();//Point
            bool okValue1;
            bool okValue2;
            xValue.toDouble(&okValue1);
            yValue.toDouble(&okValue2);
            if(!okValue1 || !okValue2){
                std::cout << "input malformed" <<std::endl;
                fileMalformed = true;
            }
        }
    }
    writer.writeEndElement();//PointMap

    writer.writeStartElement("FileMalformed");
    writer.writeAttribute("bool",QString::number(fileMalformed));
    writer.writeEndElement();//FileMalformed
    writer.writeEndElement();//Command
    writer.writeEndDocument();//end

    saveFile.close();

    return !fileMalformed;

}

//create folder for saving within
int GuiLoadSave::createProjectDirectory(QString projectName){
    //create main ProjectFiles folder if it does not exist.
    if(!QDir(QString("ProjectFiles")).exists()){
        if(!QDir().mkdir(QString("ProjectFiles"))){
            std::cout << "failed to create ProjectFiles folder" <<std::endl;
            return 1;
        }
    }

    //check if the project already exists.  return code 2 if exists.
    if(QDir(QString("ProjectFiles/").append(projectName)).exists()){
        return 2;

    }else{
        //create new folder and return 0 if successful.
        if(QDir().mkdir(QString("ProjectFiles/").append(projectName))){
        return 0;
        }else{
            return 1;
        }
    }
}

//puts info from xml into command editor.
int GuiLoadSave::updateCommandEditor(QString fileName, QString projectName, QWidget *commandEditor, MainWindow *mainWindow){

    QList<QLineEdit *> lineEdits = commandEditor->findChildren<QLineEdit *>();

    //load file and set up the reader.
    QFile loadFile;
    loadFile.setFileName(QString("ProjectFiles/") + projectName+ QString("/") + fileName+ QString(".xml"));
    loadFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&loadFile);

    reader.readNextStartElement();
    reader.readNextStartElement();

    int i = 0;

    QList<QComboBox *> comboBoxes = commandEditor->findChildren<QComboBox *>();
    lineEdits.at(2)->setText(fileName);
    foreach(const QXmlStreamAttribute &attr, reader.attributes()){
        comboBoxes.at(i)->setCurrentIndex(comboBoxes.at(i)->findText(attr.value().toString()));
        i++;
    }
    i = 0;
    //delete all buttons except for the first two.  two extra added "just in case".
    int numChildrenToRemove = (commandEditor->findChildren<QLineEdit *>().size());
    for(int l = 0; l < numChildrenToRemove; l++){
        mainWindow->on_RemovePointButton_clicked();
    }

    int k = 0;

    while(!reader.isEndDocument()){
        reader.readNext();
        if(reader.isStartElement()){
            QString pointString = "";
            if(reader.name().toString() == "PointMap"){
                int numPoints = reader.attributes().at(0).value().toString().toInt();
                for(int j = 0; j < (numPoints - 2); j++){
                    mainWindow->on_AddPointButton_clicked();

                }
                lineEdits = commandEditor->findChildren<QLineEdit *>();
                std::cout << "number of coordinates to load : " << numPoints << std::endl;
            }
            if(reader.name().toString() == "Point"){
                foreach(const QXmlStreamAttribute &attr, reader.attributes()){
                    pointString.append(attr.value().toString());
                    pointString.append(",");
                }
                pointString.chop(1);

                commandEditor->findChildren<QLineEdit *>().at(k)->setText(pointString);
                k++;
                if(k == 2){
                    k++;
                }


            } else if(reader.name().toString() == "FileMalformed"){
                if(reader.attributes().value(0).toString() == "1"){
                    std::cout << "FILE WAS MALFORMED!" << std::endl;
                }
            }

        }
    }
    if(reader.hasError()){
        std::cout << "there was an error in reading the file" <<std::endl;
    }

    loadFile.close();
    return 0;
}

