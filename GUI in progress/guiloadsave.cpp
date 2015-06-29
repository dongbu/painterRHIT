#include "guiloadsave.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

GuiLoadSave::GuiLoadSave()
{
    //empty since all functions are static.
}

/**
 * @brief properly saves command as xml and adds to list.
 * @param ProjectName
 * @param CommandEditor
 * @param CommandList
 * @return boolean for success
 */
boolean GuiLoadSave::writeCommandToFolder(QString ProjectName, QWidget *CommandEditor, QListWidget *CommandList, boolean commandAdded){

    //boolean checker to make sure the file inputs are correct.
    bool fileMalformed = false;

    //gets all the line edit pieces and puts them into an array
    QList<QLineEdit *> lineEdits = CommandEditor->findChildren<QLineEdit *>();

    //gets all the combobox pieces and puts them into an array
    QList<QComboBox *> comboBoxes = CommandEditor->findChildren<QComboBox *>();

    //the first object is the filename.  makes sure it is
    QString fileName = lineEdits.at(0)->text();


    //adds file stuff to the commandlist. Only if it wasn't added before.
    ///TEMP SOLUTION///
    if(!commandAdded){
        CommandList->addItem(fileName);
    }
    ///TEMP SOLUTION///



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
    for(int i = 1; i < lineEdits.count(); i++){
        QString point = lineEdits.at(i)->text();
        QStringList xy = point.split(',');
        QString xValue = xy.value(0);
        QString yValue = xy.value(1);
        //checks to make sure input is valid
        bool okValue1;
        bool okValue2;
        xValue.toDouble(&okValue1);
        yValue.toDouble(&okValue2);

        if(okValue1 && okValue2){
            //only writes the input into the file if it is valid.
            writer.writeStartElement("Point");
            writer.writeAttribute("x",xValue);
            writer.writeAttribute("y",yValue);
            writer.writeEndElement();//Point

        }
    }
    writer.writeEndElement();//PointMap

    writer.writeStartElement("FileMalformed");
    writer.writeAttribute("bool",QString::number(fileMalformed));
    writer.writeEndElement();//FileMalformed
    writer.writeEndElement();//Command
    writer.writeEndDocument();//end

    //always close files when done.
    saveFile.close();

    return !fileMalformed;

}


/**
 * @brief creates folder for saving within.
 * @param projectName
 * @return 0 if successful
 */
int GuiLoadSave::createProjectDirectory(QString projectName){
    //create main ProjectFiles folder if it does not exist.
    if(!QDir(QString("ProjectFiles")).exists()){
        if(!QDir().mkdir(QString("ProjectFiles"))){
            std::cout << "failed to create ProjectFiles folder" <<std::endl;
            return 1;
        }
    }

    //check if the project already exists.  return code 2 if exists.
    if(QDir(QString("ProjectFiles/") + (projectName)).exists()){
        return 2;

    }else{
        //create new folder and return 0 if successful.
        if(QDir().mkdir(QString("ProjectFiles/")+(projectName))){
            return 0;
        }else{
            return 1;
        }
    }
}


/**
 * @brief puts info from xml into command editor
 * @param fileName
 * @param projectName
 * @param editorTabs
 * @return 0 if successful
 */
int GuiLoadSave::updateCommandEditor(QString fileName, QString *projectName, CommandEditor *loadedEditor){
    loadedEditor->setName(fileName);

    ///TEMP SOLUTION///
    loadedEditor->setCommandAdded(true);
    ///TEMP SOLUTION///
    /// \brief lineEdits

    QList<QLineEdit *> lineEdits = loadedEditor->CommandEditorWidget->findChildren<QLineEdit *>();

    QString tempProjectName;
    if(projectName->isEmpty() || projectName->isNull()){
        tempProjectName = "Temp";
    }else{
        tempProjectName = *projectName;
    }


    //load file and set up the reader.
    QFile loadFile;
    loadFile.setFileName(QString("ProjectFiles/") + tempProjectName+ QString("/") + fileName+ QString(".xml"));
    loadFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&loadFile);

    //skip over unimportant doc headers.
    reader.readNextStartElement();
    reader.readNextStartElement();

    int i = 0;

    //get list of comboBoxes
    QList<QComboBox *> comboBoxes = loadedEditor->CommandEditorWidget->findChildren<QComboBox *>();

    //set text for the command name slot.
    lineEdits.at(0)->setText(fileName);

    //set the information for both comboboxes.
    foreach(const QXmlStreamAttribute &attr, reader.attributes()){
        comboBoxes.at(i)->setCurrentIndex(comboBoxes.at(i)->findText(attr.value().toString()));
        i++;
    }


    i = 0;

    int k = 1;


    //keep going until the document is finished.
    while(!reader.isEndDocument()){
        reader.readNext();
        if(reader.isStartElement()){
            QString pointString = "";
            if(reader.name().toString() == "PointMap"){
                //find and add the correct number of points.
                int numPoints = reader.attributes().at(0).value().toString().toInt();
                for(int j = 0; j < (numPoints - 2); j++){
                    loadedEditor->Add_Point_Clicked();

                }
                lineEdits = loadedEditor->CommandEditorWidget->findChildren<QLineEdit *>();
//                std::cout << "number of coordinates to load : " << numPoints << std::endl;
            }
            if(reader.name().toString() == "Point"){
                foreach(const QXmlStreamAttribute &attr, reader.attributes()){
                    pointString.append(attr.value().toString());
                    pointString.append(",");
                }
                pointString.chop(1);

                loadedEditor->CommandEditorWidget->findChildren<QLineEdit *>().at(k)->setText(pointString);
                k++;

            } else if(reader.name().toString() == "FileMalformed"){
                if(reader.attributes().value(0).toString() == "1"){
                    std::cout << "FILE WAS MALFORMED!" << std::endl;
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
    return 0;
}


/**
 * @brief puts the list of commands into an xml file.
 * @param ProjectName
 * @param CommandList
 * @return 1 if successful
 */
int GuiLoadSave::writeCommandListToFolder(QString ProjectName, QListWidget *CommandList){
    //set up writer
    QString fileLoc = QString("ProjectFiles/") + ProjectName + QString("/index.xml");
    QFile saveIndexFile;
    saveIndexFile.setFileName(fileLoc);
    saveIndexFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QXmlStreamWriter writer(&saveIndexFile);

    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeComment("index");

    writer.writeStartElement("index");

    //get all the command names.
    QList<QListWidgetItem *> listOfCommandNames = CommandList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    //put the command names into the proper place.
    for(int i = 0; i < listOfCommandNames.size(); i++){
    writer.writeStartElement(listOfCommandNames.at(i)->text());//index at
    writer.writeEndElement();//index at
//    std::cout << listOfCommandNames.at(i)->text().toStdString() << std::endl;
    }

    writer.writeEndElement();//index
    writer.writeEndDocument();//end of doc

    int error = !writer.hasError();
    saveIndexFile.close();

    //one if no error, zero if error
    return(error);

}



/**
 * @brief loads the list of commands from an xml into the proper place
 * @param ProjectName
 * @param CommandList
 * @return 1 if successful
 */
int GuiLoadSave::loadCommandListFromFolder(QString ProjectName, QListWidget *CommandList){
    //setup reader
    QString fileLoc = QString("ProjectFiles/") + ProjectName + QString("/index.xml");
    QFile loadIndexFile;
    loadIndexFile.setFileName(fileLoc);
    if(!loadIndexFile.exists()){
        return 0;
    }
    loadIndexFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&loadIndexFile);

    //keep reading until reach the end.
    while(!reader.isEndDocument()){
        reader.readNext();
//        std::cout << reader.name().toString().toStdString() << std::endl;
        if(reader.isStartElement()){
            QString text;
                text = reader.name().toString();
            if(text != "index"){
                //add item to list.
                CommandList->addItem(text);
            }
        }
    }
    int error = !reader.hasError();
    loadIndexFile.close();
    //one if no error, zero if error
    return(error);
}


/**
 * @brief calls up a fileDialog box with which to choose and save as a filename.
 * @return projectName
 */
QString GuiLoadSave::saveAsProject(){
    QMessageBox alert;

    QFileDialog saveDirectory;
    saveDirectory.setDirectory("ProjectFiles");
    saveDirectory.setAcceptMode(QFileDialog::AcceptSave);
    QStringList filters;
    filters << "Text files (*.txt)";
    saveDirectory.setNameFilters(filters);

    if(saveDirectory.exec()){
        QString name = saveDirectory.selectedFiles().at(0).split("/").last();
        name.chop(4);
        //makes sure temp is not used as a folder name.
        if(name.toLower() == "temp"){
            QMessageBox alert;
            alert.setText("Alert");
            alert.setInformativeText("temp is protected");
            if(alert.exec()){
                return "";
            }
        }
        if (!name.isEmpty() && name.isSimpleText() && !name.contains(QRegExp("[" + QRegExp::escape("\\/:*?\"<>|") + "]"))){
            //creates proper folders
            int loadReturnCode = GuiLoadSave::createProjectDirectory(name);
            //return code 0 means it worked.
            if(loadReturnCode != 0){
                //ProjectFiles folder could not be created
                if(loadReturnCode == 1){
                    alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                    alert.setInformativeText("Failed To Create ProjectFiles directory");
                    alert.show();
                    return "";
                }
                //project already exists and overwrite was confirmed
                else if(loadReturnCode == 2){
                    QDir dir(QString("ProjectFiles/") + name);
                    if(dir.exists()){
                        if(!dir.removeRecursively()){
                            std::cout << "failed to remove previous folder" <<std::endl;
                            return 0;
                        }
                        GuiLoadSave::createProjectDirectory(name);

                        return name;

                    }else{
                        std::cout << name.toStdString() << " does not exist!" << std::endl;
                        return "";
                    }
                }

            } else{
                return name;
            }


        }else{
            //basically user put in a bad filename
            alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
            alert.setInformativeText(name + " is not a valid name");
            alert.show();
            return "";
        }
    }
    return "";

}


/**
 * @brief copies all files (except index.xml and the .txt file) from one file into the other.
 * @param prevFolder
 * @param newFolder
 * @return true if successful
 */
boolean GuiLoadSave::copyAllFilesFrom(QString prevFolder, QString newFolder){

    //iterator
    QDirIterator it("ProjectFiles/" + prevFolder);

    while(it.hasNext()){
        QFile temp;
        it.next();
        QString extension = it.fileName().split(".").last();
        temp.setFileName("ProjectFiles/" + prevFolder + "/" + it.fileName());
        if(temp.exists() && it.fileName() != "." && it.fileName() != ".." && it.fileName() != "index.xml" && extension != "txt"){
            if(!temp.copy(temp.fileName(),"ProjectFiles/" + newFolder + "/" + temp.fileName().split("/").last()) && extension == "xml"){
                //problem with copying.
                return false;
            }
        }else{

            //file does not exist, thus skip over.
        }

        temp.close();

    }
    return true;
}

/**
 * @brief used to load a command into the commandlist regardless of project origin.
 * @param projectName
 * @param CommandList
 * @return true if successful
 */
boolean GuiLoadSave::loadExternalFile(QString projectName, QListWidget *CommandList){
    //file directory
    QFileDialog loadDirectory;
    loadDirectory.setDirectory("ProjectFiles");
    loadDirectory.setAcceptMode(QFileDialog::AcceptOpen);
    QStringList filters;
    filters << "XML files (*.xml)";
    loadDirectory.setNameFilters(filters);


    if(loadDirectory.exec()){
        QString name = loadDirectory.selectedFiles().at(0).split("/").last();
        name.chop(4);
        //ensure selected file is not index.
        if(name.toLower() == "index"){
            QMessageBox alert;
            alert.setText("Alert");
            alert.setInformativeText("not a valid file!");
            alert.show();
            return false;
        }
        QString prevFolder = loadDirectory.selectedFiles().at(0).split("ProjectFiles/").last();


        //file is in same project.  just add filename to list.
        if(prevFolder.split("/").first() == projectName){
            CommandList->addItem(name);
            return true;
        }else{
            //must copy file from prevFolder to new folder.
            QFile temp;
            temp.setFileName("ProjectFiles/" + prevFolder);
            if(temp.copy(temp.fileName(),"ProjectFiles/" + projectName + "/" + name + ".xml")){
                CommandList->addItem(name);
                return true;
            }
            return false;

        }

    }
    return false;
}


std::vector<std::vector<int> > getPointVectors(QString projectName, QListWidget* CommandList){
    std::vector<std::vector<int> > result;

    std::vector<int> x1;
    std::vector<int> x2;
    std::vector<int> y1;
    std::vector<int> y2;
    int prevX;
    int prevY;

    QList<QListWidgetItem *> listOfCommands = CommandList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    for(int i = 0; i < listOfCommands.length(); i++){

    QFile loadFile;
    loadFile.setFileName(QString("ProjectFiles/") + projectName+ QString("/") + listOfCommands.at(i)->text()+ QString(".xml"));
    loadFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&loadFile);

    //skip over unimportant doc headers.
    reader.readNextStartElement();
    reader.readNextStartElement();


    //keep going until the document is finished.
    while(!reader.isEndDocument()){
        reader.readNext();
        if(reader.isStartElement()){
            if(reader.name().toString() == "Point"){
                int k = 0;
                foreach(const QXmlStreamAttribute &attr, reader.attributes()){
                    if(x1.size() <= 0){
                        if(k == 0){
                            prevX = attr.value().toInt();
                        }else{
                            prevY = attr.value().toInt();
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
                        }
                    }
                    k++;
                }


            } else if(reader.name().toString() == "FileMalformed"){
                if(reader.attributes().value(0).toString() == "1"){
                    std::cout << "FILE WAS MALFORMED!" << std::endl;
                    return result;
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

    result.push_back(x1);
    result.push_back(y1);
    result.push_back(x2);
    result.push_back(y2);
    return result;
}

