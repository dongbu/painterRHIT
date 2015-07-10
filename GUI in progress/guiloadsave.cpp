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
 * @param Line
 * @param CommandList
 * @return boolean for success
 */
bool GuiLoadSave::writeCommandToFolder(QString fileLocation, QWidget *Line, QListWidget *CommandList, bool commandAdded, QString commandType){
	QString extension = fileLocation.split('.').last();
	if (extension == "xml" || extension == "txt"){
		fileLocation.chop(4);
	}

	printf("fileLocation for writing command: %s\n", fileLocation.toStdString().c_str());
    //boolean checker to make sure the file inputs are correct.
    bool fileMalformed = false;

    //gets all the line edit pieces and puts them into an array
    QList<QLineEdit *> lineEdits = Line->findChildren<QLineEdit *>();

    //gets all the combobox pieces and puts them into an array
    QList<QComboBox *> comboBoxes = Line->findChildren<QComboBox *>();

    //gets the spinbox with lineWidth information
    QList<QSpinBox *> spinBoxes = Line->findChildren<QSpinBox *>();
    QString lineWidth = spinBoxes.at(0)->text();
    if(lineWidth == ""){
        lineWidth = "0";
    }

    //the first object is the filename.  makes sure it is
    QString fileName = lineEdits.at(0)->text();

    //adds file stuff to the commandlist. Only if it wasn't added before.
    if(!commandAdded){
        CommandList->addItem(fileName);
    }

    //sets up a save file to put the information into.  Should overwrite any previous file with same name in directory.
    QString fileLoc = fileLocation + QString("/") + fileName + QString(".xml");
	if (fileLocation == ""){
		fileLoc = QString("ProjectFiles/Temp/") + fileName + QString(".xml");
	}
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
	writer.writeAttribute("type", commandType);

    //deals with line info
    writer.writeComment("Line Information");
    writer.writeStartElement("Line");
    writer.writeAttribute("color",comboBoxes.at(0)->property("currentText").toString());
    writer.writeAttribute("style", comboBoxes.at(1)->property("currentText").toString());
    writer.writeAttribute("width",lineWidth);
    writer.writeEndElement();//Line

    //deals with pointmap
    writer.writeComment("Point Map Information");
    writer.writeStartElement("PointMap");
    writer.writeAttribute("length",QString::number(lineEdits.count()-1));
    for(int i = 2; i < lineEdits.count(); i++){
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

	if (writer.hasError()){
		printf("writer in writeCommandToFolder has error\n");
	}

    //always close files when done.
    saveFile.close();

    return !fileMalformed;
}


/**
 * @brief creates folder for saving within.
 * @param projectName
 * @return 0 if successful
 */
int GuiLoadSave::createProjectDirectory(QString saveLocation){
    //check if the project already exists.  return code 2 if exists.
	QString extension = saveLocation.split('.').last();
	if (extension == "xml" || extension == "txt"){
		saveLocation.chop(4);
	}
    if(QDir(saveLocation).exists()){
        return 2;

    }else{
        //create new folder and return 0 if successful.
        if(QDir().mkdir(saveLocation)){
            return 0;
        }else{
            return 1;
        }
    }
}

/**
 * @brief puts the list of commands into an xml file.
 * @param ProjectName
 * @param CommandList
 * @return 1 if successful
 */
int GuiLoadSave::writeCommandListToFolder(QString fileLocation, QListWidget *CommandList){
    //set up writer
	QString extension = fileLocation.split('.').last();
	if (extension == "xml" || extension == "txt"){
		fileLocation.chop(4);
	}
    QString fileLoc = fileLocation + QString("/index.xml");
	printf("%s\n", fileLocation);
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
int GuiLoadSave::loadCommandListFromFolder(QString fileLocation, QListWidget *CommandList){
	QString extension = fileLocation.split('.').last();
	if (extension == "xml" || extension == "txt"){
		fileLocation.chop(4);
	}
	int difference = fileLocation.split("/").last().length();
	fileLocation.chop(difference + 1);
    //setup reader
    QString fileLoc = fileLocation + QString("/index.xml");
	printf("loadCommandListFromFolder fileLoc: %s\n", fileLoc.toStdString().c_str());
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
 * @return project location
 */
QString GuiLoadSave::saveAsProject(QString saveLocation){
    QMessageBox alert;

    QFileDialog saveDirectory;
	if (saveLocation == ""){
		saveDirectory.setDirectory("ProjectFiles");
	}
	else{
		int difference = saveLocation.split("/").last().length();
		QString temp = saveLocation.mid(0, saveLocation.length() - difference);
		saveDirectory.setDirectory(temp);
	}
    saveDirectory.setAcceptMode(QFileDialog::AcceptSave);
    QStringList filters;
    filters << "Text files (*.txt)";
    saveDirectory.setNameFilters(filters);

    if(saveDirectory.exec()){
        QString name = saveDirectory.selectedFiles().at(0).split("/").last();
        name.chop(4);
		QString saveLoc = saveDirectory.selectedFiles().at(0);
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
            int loadReturnCode = GuiLoadSave::createProjectDirectory(saveLoc);
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
                        GuiLoadSave::createProjectDirectory(saveLoc);

                        return saveDirectory.selectedFiles().at(0);

                    }else{
                        std::cout << name.toStdString() << " does not exist!" << std::endl;
                        return "";
                    }
                }

            } else{
                return saveLoc;
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
bool GuiLoadSave::copyAllFilesFrom(QString prevLocation, QString newLocation){
	QString extension1 = prevLocation.split('.').last();
	if (extension1 == "xml" || extension1 == "txt"){
		prevLocation.chop(4);
	}
	QString extension2 = newLocation.split('.').last();
	if (extension2 == "xml" || extension2 == "txt"){
		newLocation.chop(4);
	}

    //iterator
    QDirIterator it(prevLocation);

    while(it.hasNext()){
        QFile temp;
        it.next();
        QString extension = it.fileName().split(".").last();
        temp.setFileName(prevLocation + "/" + it.fileName());
        if(temp.exists() && it.fileName() != "." && it.fileName() != ".." && it.fileName() != "index.xml" && extension != "txt"){
            if(!temp.copy(temp.fileName(),newLocation + "/" + temp.fileName().split("/").last()) && extension == "xml"){
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
bool GuiLoadSave::loadExternalFile(QString fileLocation, QListWidget *CommandList){
    //file directory
    QFileDialog loadDirectory;
	int difference = fileLocation.split("/").last().length();
	QString temp = fileLocation.mid(0, fileLocation.length() - difference);
	loadDirectory.setDirectory(temp);
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
		QString prevFolder = loadDirectory.selectedFiles().at(0);


        //file is in same project.  just add filename to list.
        if(prevFolder == fileLocation){
            CommandList->addItem(name);
            return true;
        }else{
            //must copy file from prevFolder to new folder.
            QFile temp;
            temp.setFileName(prevFolder);
            if(temp.copy(temp.fileName(),fileLocation + "/" + name + ".xml")){
                CommandList->addItem(name);
                return true;
            }
            return false;

        }

    }
    return false;
}

