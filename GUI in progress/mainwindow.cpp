#include "mainwindow.h"
#include "QDebug.h"
#include <QRadioButton>
/**
 * @brief main window that does everything.
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	editorWorks = false;
	saveLocation = "";

	//Robot work//
	this->robot = new Robot();
	//Robot work//

	//window work//
	this->setWindowTitle("Robot Artist 3000 Deluxe Gold Extreme Edition");
	//window work//

    //save work//
    projectName = ""; //"garbage" value
    saved = false;
	this->fileChangedFalse();
    //creates a ProjectFiles folder if one doesn't exist
    if(!QDir(QString("ProjectFiles")).exists()){
        if(!QDir().mkdir(QString("ProjectFiles"))){
            std::cout << "failed to create ProjectFiles folder" <<std::endl;
        }
    }
    //save work//

    //command list//
    commandView = new CommandViewer();
    commandView->setProjectName(&projectName);
	commandView->setProjectLocation(&saveLocation);
	commandView->setRobot(this->robot);
    connect(this,SIGNAL(sendSaved(bool)),commandView,SLOT(fileSaved(bool)));
    connect(commandView,SIGNAL(fileStatusChanged()),this,SLOT(fileChangedTrue()));
    connect(commandView,SIGNAL(EmitConnectEditor(Line*)),this,SLOT(ConnectEditor(Line*)));
	connect(commandView, (SIGNAL(MustSave())), this, SLOT(saveTempIndex()));
    //command list//

    //click to draw work//
	drawOn2 = new drawOnWidget(ui->widget, 1);
	drawOn2->setFixedWidth(1000);
	drawOn2->setFixedHeight(750);
	drawOn2->projectName = projectName;
	ui->widget->setStyleSheet("background-color:rgba(255,255,255,0);");


    drawOn = new drawOnWidget(ui->widget, 0);
    drawOn->setFixedWidth(1000);
    drawOn->setFixedHeight(750);
	drawOn->projectName = projectName;
    connect(drawOn,SIGNAL(sendPoint(int, int, int)),this,SLOT(recievePoint(int, int, int)));
    ui->widget->setStyleSheet("background-color:rgba(255,255,255,0);");
    ui->DrawFunctions->setHidden(true);

	drawOn->setRobot(this->robot);
	drawOn2->setRobot(this->robot);
	//click to draw work//

    //line options work//
	colorBox = new QComboBox();
	styleBox = new QComboBox();
	thicknessBox = new QSpinBox();
    QStringList colors,styles;
    colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";
    styles << "solid" << "dashed" << "dashed dot";
    colorBox->addItems(colors);
    styleBox->addItems(styles);

	thicknessBox->setFixedWidth(60);
    thicknessBox->setMinimum(1);
    thicknessBox->setMaximum(20);
    thicknessBox->setSingleStep(1);
    thicknessBox->setValue(4);

    ui->drawingToolbar->addWidget(colorBox);
    ui->drawingToolbar->addWidget(styleBox);
    ui->drawingToolbar->addWidget(thicknessBox);
    connect(colorBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_drawing_changed()));
    connect(styleBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_drawing_changed()));
    connect(thicknessBox,SIGNAL(valueChanged(int)),this,SLOT(on_drawing_changed()));
    //line options work//

    //robot connection work//
    connect(this,SIGNAL(makeConnection(QString)),this->commandView->interpreter,SLOT(beginConnecting(QString)));
    //robot connection work//

	//refresh work//
	QAction *refresh = new QAction(this);
	refresh->setText("refresh");
	connect(refresh, SIGNAL(triggered()), this, SLOT(drawOn2_update()));
	ui->drawingToolbar->addAction(refresh);
	//refresh work//

    //webcam work//
    QAction *startWebcam = new QAction(this);
    startWebcam->setText("camera");
    connect(startWebcam,SIGNAL(triggered()),this,SLOT(openCamera()));
    ui->miscToolbar->addAction(startWebcam);
    //webcam work//

    this->move(700, 100);

}

/**
 * @brief Default after close all functions (when you press "X").
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Save As functionality.
 */
void MainWindow::on_actionSave_As_triggered()
{
    //make sure ProjectFiles folder exists
    if(!saved){
        //saveAsProject() returns the name that was chosen to save the project under.
        saveLocation = GuiLoadSave::saveAsProject(saveLocation);
		saveLocation.chop(4);
		QString name = saveLocation.split("/").last();
        if(!name.isEmpty()){
            saved = true;
            projectName = name;
			drawOn->projectName = projectName;
			drawOn2->projectName = projectName;
            this->setWindowTitle(projectName);
            commandView->setProjectName(&projectName);
            ui->actionSave->setEnabled(true);

            //chunks in index.xml file
            if(!GuiLoadSave::writeCommandListToFolder(saveLocation, this->commandView->list)){
                alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                alert.setInformativeText("Failed To Create " + projectName + "/index.xml");
                if(alert.exec()){
                    return;
                }
            }

            //creates the "dummy" file that is used for clicking.
            QFile dummy;
            dummy.setFileName(saveLocation + QString("/") + name + QString(".txt"));
            dummy.open(QIODevice::WriteOnly);
            dummy.close();
            emit sendSaved(true);
			this->fileChangedFalse();

            //moves all files from temp folder into current folder if temp folder exists.  Also deletes temp folder.
            if(QDir("ProjectFiles/Temp").exists()){
                if(!GuiLoadSave::copyAllFilesFrom("ProjectFiles/Temp",saveLocation)){
                    std::cout << "Problem with Temp" << std::endl;
                }else{
                    QDir("ProjectFiles/Temp").removeRecursively();
                }
            }
        }
    }else{
        //has been saved before.
        QString prevProjectLocation = saveLocation;
        //same as above.
		saveLocation = GuiLoadSave::saveAsProject(saveLocation);
		saveLocation.chop(4);
		QString name = saveLocation.split("/").last();

        if(!name.isEmpty()){
            saved = true;
            projectName = name;
            this->setWindowTitle(projectName);
            ui->actionSave->setEnabled(true);
            commandView->setProjectName(&projectName);
			drawOn->projectName = projectName;
			drawOn2->projectName = projectName;

            //chunks in index.xml file
            if(!GuiLoadSave::writeCommandListToFolder(saveLocation, this->commandView->list)){
                alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
                alert.setInformativeText("Failed To Create " + saveLocation + "/index.xml");
                if(alert.exec()){
                    return;
                }
            }
            //creates the "dummy" file that is used for clicking.
            QFile dummy;
            dummy.setFileName(saveLocation + QString("/") + name + QString(".txt"));
            dummy.open(QIODevice::WriteOnly);
            dummy.close();
            emit sendSaved(true);

            //transfers all files over from the previous location to the new location.
            if(!GuiLoadSave::copyAllFilesFrom(prevProjectLocation, saveLocation)){
                std::cout << "something went wrong transfering files from " <<prevProjectLocation.toStdString() << " to " << saveLocation.toStdString() << std::endl;
            }else{
                this->fileChangedFalse();
            }
        }
    }
}

/**
 * @brief opening project functionality
 */
void MainWindow::on_actionOpen_triggered()
{
    //asks you if you want to save your work before opening something new.
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);
        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            if(saved){
                MainWindow::on_actionSave_triggered();
            }else{
                MainWindow::on_actionSave_As_triggered();
            }
            if(QDir("ProjectFiles/Temp").exists()){
                QDir("ProjectFiles/Temp").removeRecursively();
            }
            break;
        case QMessageBox::Discard:
            if(QDir("ProjectFiles/Temp").exists()){
                QDir("ProjectFiles/Temp").removeRecursively();
            }
            break;
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Default:
            //should not get here
            return;
        }
    }

    //opens up a directory viewer that only shows folders and .txt files.
    QFileDialog directory;
	if (saveLocation == ""){
		directory.setDirectory("ProjectFiles");
	}
	else{
		directory.setDirectory(saveLocation);
	}
    QStringList filters;
    filters << "Text files (*.txt)";
    directory.setNameFilters(filters);
    if(directory.exec()){
        MainWindow::cleanUp();

		saveLocation = directory.selectedFiles().at(0);
		saveLocation.chop(4);
        projectName = saveLocation.split("/").last();

		saveLocation.chop(projectName.length());
		
		printf("opening at location: %s\n", saveLocation.toStdString().c_str());
        commandView->setProjectName(&projectName);
		drawOn->projectName = projectName;
		drawOn2->projectName = projectName;
		commandView->setProjectLocation(&saveLocation);
        if(!GuiLoadSave::loadCommandListFromFolder(saveLocation,this->commandView->list)){
            alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
            alert.setInformativeText("Failed To Load " + saveLocation + "/index.xml");
            alert.show();
        }else{
            this->setWindowTitle(projectName);
            saved=true;
            ui->actionSave->setEnabled(true);
            emit sendSaved(true);
        }
    }
}

/**
 * @brief normal save functionality
 */
void MainWindow::on_actionSave_triggered()
{
    if(!saved){
        MainWindow::on_actionSave_As_triggered();
        return;
    }
    if(!GuiLoadSave::writeCommandListToFolder(saveLocation,this->commandView->list)){
        alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
        alert.setInformativeText("Failed To Save " + projectName);
        alert.show();
    }
	this->fileChangedFalse();
    emit sendSaved(true);
}

/**
 * @brief removes all project specific variables and clears away
 * lists.
 */
void MainWindow::cleanUp(){
	this->fileChangedFalse();
    this->saved = false;
    this->setWindowTitle("Untitled");
    this->projectName = "";
    ui->actionSave->setDisabled(true);
    if(QDir("ProjectFiles/Temp").exists()){
        QDir("ProjectFiles/Temp").removeRecursively();
    }
    drawOn->clearAll(1);
	drawOn2->clearAll(1);
	this->colorBox->setCurrentIndex(0);
	this->styleBox->setCurrentIndex(0);
	this->thicknessBox->setValue(4);
    emit sendSaved(false);

	//unconnect and destroy old commandView.
	disconnect(this, SIGNAL(sendSaved(bool)), commandView, SLOT(fileSaved(bool)));
	disconnect(commandView, SIGNAL(fileStatusChanged()), this, SLOT(fileChangedTrue()));
	disconnect(commandView, SIGNAL(EmitConnectEditor(Line*)), this, SLOT(ConnectEditor(Line*)));
	disconnect(commandView, (SIGNAL(MustSave())), this, SLOT(saveTempIndex()));
	disconnect(this, SIGNAL(makeConnection(QString)), this->commandView->interpreter, SLOT(beginConnecting(QString)));
	commandView->setMainClosed(true);
	commandView->close();

	//create and reconnect new commandView
	commandView = new CommandViewer();
	commandView->setProjectName(&projectName);
	commandView->setProjectLocation(&saveLocation);
	connect(this, SIGNAL(sendSaved(bool)), commandView, SLOT(fileSaved(bool)));
	connect(commandView, SIGNAL(fileStatusChanged()), this, SLOT(fileChangedTrue()));
	connect(commandView, SIGNAL(EmitConnectEditor(Line*)), this, SLOT(ConnectEditor(Line*)));
	connect(commandView, (SIGNAL(MustSave())), this, SLOT(saveTempIndex()));
	connect(this, SIGNAL(makeConnection(QString)), this->commandView->interpreter, SLOT(beginConnecting(QString)));
	
}

/**
 * @brief slot that sets fileChanged to true
 */
void MainWindow::fileChangedTrue(){
    fileChanged = true;
	this->commandView->fileChanged = true;
}

/**
* @brief slot that sets fileChanged to false
*/
void MainWindow::fileChangedFalse(){
	fileChanged = false;
	this->commandView->fileChanged = false;
}

/**
 * @brief slot for a new project
 */
void MainWindow::on_actionNew_triggered()
{
    //asks you if you want to save your work before starting something new.
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);
        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:
            if(saved){
                MainWindow::on_actionSave_triggered();
            }else{
                MainWindow::on_actionSave_As_triggered();
            }
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Default:
            //should not get here
            return;
        }
    }
    MainWindow::cleanUp();
}

/**
 * @brief slot for DrawOnWidget to get points that were clicked.
 * @param x
 * @param y
 */
void MainWindow::recievePoint(int x, int y, int pointCount){
    //means command is over.
	editorWorks = true;
    if(x == -10 && y == -10){
        if(pointCount >= 3){
            Line *temp = commandView->currentEditor;
            temp->add_Command_Externally(this->projectName);
			drawOn->clearAll(1);
            return;
        }else{
            drawOn->clearAll(1);
            return;
        }
    }
	if (pointCount == 1){
		commandView->MakeEditor();
		colorBox->setEnabled(true);
		styleBox->setEnabled(true);
		thicknessBox->setEnabled(true);
	}
	if (pointCount == 2) emit colorBox->currentIndexChanged(colorBox->currentIndex());
    Line *temp = commandView->currentEditor;
    if(pointCount > 2){
        temp->Add_Point_Clicked();
    }
    temp->set_Point_At(pointCount + 1, x, y);
}

void MainWindow::noticeCommandAdded(int index){
    if(index == -10){
        drawOn->clearAll(1);
		editorWorks = false;
    }
}

/**
 * @brief closes all the windows when this window is closed.
 * also prompts for a save.
 */
void MainWindow::closeEvent(QCloseEvent *event){
    //if the file was changed and not saved, asks you if you would like to save
    //or discard changes.
    if(fileChanged){
        QMessageBox queryUnsavedWork;
        queryUnsavedWork.setText("The document has been modified");
        queryUnsavedWork.setInformativeText("Would you like to save your changes?");
        queryUnsavedWork.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        queryUnsavedWork.setDefaultButton(QMessageBox::Save);

        int result = queryUnsavedWork.exec();

        switch(result){
        case QMessageBox::Save:

            if(saved){
                MainWindow::on_actionSave_triggered();
            }else{
                MainWindow::on_actionSave_As_triggered();
            }
            event->accept();
            break;
        case QMessageBox::Discard:
            
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        case QMessageBox::Default:
            //If the user manages to click some other button, return.
            event->ignore();
            return;
        }
    }
    commandView->setMainClosed(true);
    commandView->close();
	if (QDir("ProjectFiles/Temp").exists()){
		QDir("ProjectFiles/Temp").removeRecursively();
	}
}

/**
 * @brief connects the given editor to the appropriate slots.
 * @param editor
 */
void MainWindow::ConnectEditor(Line* editor) {
    //connection so that we know when something has been changed.
    connect(editor,SIGNAL(fileStatusChanged()),this,SLOT(fileChangedTrue()));

    //connection so we know if addcommand button was pressed.
    connect(editor,SIGNAL(tell_Command_Added(int)),this,SLOT(noticeCommandAdded(int)));

    //connection to update drawOn.
    connect(editor,SIGNAL(sendUpdateToDrawOn(Line*)),drawOn,SLOT(updateToEditor(Line*)));
	connect(editor, SIGNAL(sendUpdateToDrawOn(Line*)), drawOn2, SLOT(updateToEditor(Line*)));
	//connect(editor, SIGNAL(tell_Command_Added(int)), this, SLOT(drawOn2_update()));
	connect(this, SIGNAL(sendListOfCommands(CommandViewer*, QString)), drawOn2, SLOT(updateToAllEditors(CommandViewer*, QString)));
	
}

/**
 * @brief connects ze robot.
 */
void MainWindow::on_actionConnect_triggered()
{
    QMessageBox config;
    config.setWindowTitle("Configure");
	config.setText("Configure");
	config.setInformativeText("ideally load config file here");
	QLayout *lay = config.layout();
    QRadioButton *cyton = new QRadioButton("Cyton");
    QRadioButton *ABB = new QRadioButton("ABB");
	lay->addWidget(cyton);
	lay->addWidget(ABB);
	QPushButton *connectButton = config.addButton(tr("Connect"), QMessageBox::ActionRole);
	QPushButton *abortButton = config.addButton(QMessageBox::Abort);
	config.setVisible(true);
	config.exec();

	if (config.clickedButton() == connectButton){
		printf("connecting to....\n");
		if (cyton->isChecked()){
			printf("cyton \n");
            emit makeConnection("cyton");
		}
		else if (ABB->isChecked()){
			printf("ABB \n");
            emit makeConnection("ABB");
		}

	}
    else if(config.clickedButton() == abortButton){
		printf("do nothing\n");
	}

}

/**
 * @brief updates the commands whenever the drawing is changed.
 */
void MainWindow::on_drawing_changed(){
	if (!editorWorks){
		return;
	}

	Line *editor = drawOn->currentEditor;
	connect(this, SIGNAL(sendLineStyles(QString, QString, int)), editor, SLOT(updateLineStyles(QString, QString, int)));
	QString color = colorBox->currentText();
	QString style = styleBox->currentText();
	int width = thicknessBox->text().toInt();
	emit sendLineStyles(color, style, width);
	disconnect(this, SIGNAL(sendLineStyles(QString, QString, int)), editor, SLOT(updateLineStyles(QString, QString, int)));
}

/**
 * @brief updates drawOn2
 */
void MainWindow::drawOn2_update(){
	emit sendListOfCommands(commandView, saveLocation);
}

/**
 * @brief opens le camera.
 */
void MainWindow::openCamera(){
    cam.openCamera(0);
}

void MainWindow::saveTempIndex(){
	if (!saved){
		projectName = "Temp";
		drawOn->projectName = projectName;
		drawOn2->projectName = projectName;
		commandView->setProjectName(&projectName);
		if (!GuiLoadSave::writeCommandListToFolder("ProjectFiles/Temp", this->commandView->list)){
			alert.setText("<html><strong style=\"color:red\">ERROR:</strong></html>");
			alert.setInformativeText("Failed To Create " + projectName + "/index.xml");
		}
	}
	else{
		MainWindow::on_actionSave_triggered();
	}
}