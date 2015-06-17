#include "commandeditor.h"
#include "guiloadsave.h"
#include <QTextStream>
#include <QMessageBox>

CommandEditor::CommandEditor(QListWidget *widget, int tabPosition, QTabWidget *editorTabs) {
    PointVec = new std::vector<QLineEdit*>();
    pointCount = 0;
    this->BuildEditor();
    this->ConnectButtons();
    this->name = "untitled";
    this->widget = widget;
    this->tabPosition = tabPosition;
    this->editorTabs = editorTabs;
    ///TEMP SOLUTION///
    this->commandAdded = false;
    ///TEMP SOLUTION///

}




/**
 * @brief This method is designed to create the CommandEditor widget
 * representative of this class and assign it to the appropriate
 * class variable.
 */
void CommandEditor::BuildEditor() {
    //Creating forms
    ParameterHolder = new QFormLayout();
    QGridLayout *ButtonHolder = new QGridLayout();
    QFormLayout *HolderHolder = new QFormLayout();

    //making parameterHolder form more useful
    ParameterHolder->setSizeConstraint(QLayout::SetMinimumSize);

    //Populating forms
    PopulateButtons(ButtonHolder);
    PopulateParameters(ParameterHolder);

    //Encapsulating Widgets
    QWidget *ParamWidg = new QWidget();
    QWidget *ButtonWidg = new QWidget();
    QWidget *HolderWidg = new QWidget();
    ParamWidg->setLayout(ParameterHolder);
    ButtonWidg->setLayout(ButtonHolder);
    HolderWidg->setLayout(HolderHolder);

    //Creating scrollbar
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Setting Pointers
    scrollArea->setWidget(ParamWidg);
    HolderHolder->addRow(scrollArea);
    HolderHolder->addRow(ButtonWidg);

    this->CommandEditorWidget = HolderWidg;
}




/**
 * @brief This method is designed to create a std. point box, add it to
 * the point vector, and return said point
 */
void CommandEditor::MakePoint() {
    QLineEdit *point = new QLineEdit();
    point->setPlaceholderText("0,0");
    point->setFixedSize(75,22);
    PointVec->push_back(point);
    pointCount++;
    QString label = ("Point " + QString::number(pointCount) + ":");
    ParameterHolder->addRow(new QLabel(label),point);
}




/**
 * @brief This method is designed to populate a provided layout with the 5
 * input types necessary for controlling the command editor.
 * @param ParameterHolder
 */
void CommandEditor::PopulateParameters(QFormLayout *ParameterHolder) {
    //creating input lines
    Command_Name = new QLineEdit();
    Line_Color = new QComboBox();
    Line_Style = new QComboBox();

    //forcing command name to be acceptable. Previously, entering only a number as a command name caused crash upon load.
    QRegExp rx("^[A-Za-z][A-Za-z0-9]*");
    Command_Name->setValidator(new QRegExpValidator(rx,this));

    //constraining sizes
    Command_Name->setFixedSize(200,22);
    Line_Color->setFixedSize(200,22);
    Line_Style->setFixedSize(200,22);

    //ComboBox Options
    QStringList colors,styles;
    colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";
    styles << "solid" << "dashed" << "dashed dot";
    Line_Color->addItems(colors);
    Line_Style->addItems(styles);

    //adding input lines
    ParameterHolder->addRow("Command Name: ",Command_Name);
    ParameterHolder->addRow("Color: ",Line_Color);
    ParameterHolder->addRow("Style: ",Line_Style);
    MakePoint(); //adding 1st default point
    MakePoint(); //adding 2nd default point

}


/**
 * @brief This method is designed to populate a provided layout with the 3
 * buttons necessary for controlling the command editor.
 * @param ButtonHolder
 */
void CommandEditor::PopulateButtons(QGridLayout *ButtonHolder) {
    //creating buttons
    Add_Command = new QPushButton("Add Command");
    Add_Point = new QPushButton("Add Point");
    Remove_Point = new QPushButton("Remove Point");

    //adding buttons
    ButtonHolder->addWidget(Add_Command,0,0);
    ButtonHolder->addWidget(Add_Point,0,1);
    ButtonHolder->addWidget(Remove_Point,0,2);

    Remove_Point->setDisabled(true);
}


/**
 * @brief Add_Command slot
 */

void CommandEditor::Add_Command_Clicked() {

    QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();


    //very bad if we let the user overwrite the index file.  In fact, if this occurs, it will load non-existant commands.
    //these commands then crash the program.
    if(lineEdits.at(0)->text().toLower() == "index"){
        QMessageBox alert;
        alert.setText("Alert");
        alert.setInformativeText("index is protected");
        if(alert.exec()){
            return;
        }
    }
    //make sure everything else is acceptable.
    if(projectName.isEmpty() || projectName.isNull()){
        //creates a "Temp" folder to put things into until saved.
        if(!QDir("ProjectFiles/Temp").exists()){
           QDir().mkdir("ProjectFiles/Temp");
        }
        projectName = "Temp";
    }
        lineEdits.at(0)->setDisabled(true);
        GuiLoadSave::writeCommandToFolder(projectName,this->CommandEditorWidget,widget,commandAdded);
        commandAdded = true;
        //sets the name and changes the tabname
        CommandEditor::setName(lineEdits.at(0)->text());
        editorTabs->setTabText(tabPosition,name);
        emit fileStatusChanged();
        emit tell_Command_Added();


    ///TEMP SOLUTION///
    this->Add_Command->setText("Save");

    ///TEMP SOLUTION///

}


/**
 * @brief Add_Point slot.
 */
void CommandEditor::Add_Point_Clicked() {
    Remove_Point->setDisabled(false);
    MakePoint();
}


/**
 * @brief Remove_Point slot.
 */
void CommandEditor::Remove_Point_Clicked() {
    //Button De-activation logic
    if (pointCount == 2) return;
    else if(pointCount ==3) Remove_Point->setDisabled(true);

    //class reference logic
    pointCount--;
    if(pointCount )
    PointVec->pop_back();

    //Widgit deletion logic
    int lastInput = ParameterHolder->count()-1;
    int secondLast = lastInput - 1;
    QLayoutItem *toDelete1 = ParameterHolder->itemAt(lastInput);
    QLayoutItem *toDelete2 = ParameterHolder->itemAt(secondLast);
    ParameterHolder->removeItem(toDelete2);
    ParameterHolder->removeItem(toDelete1);
    toDelete1->widget()->setVisible(false);
    toDelete2->widget()->setVisible(false);
    toDelete1->widget()->deleteLater();
    toDelete2->widget()->deleteLater();
    ParameterHolder->update();
}


/**
 * @brief This method is designed to connect the 3 editor buttons to their slots.
 */
void CommandEditor::ConnectButtons() {
    //Connecting button signals/slots
    connect(Add_Command, QPushButton::clicked, this, Add_Command_Clicked);
    connect(Add_Point, QPushButton::clicked, this, Add_Point_Clicked);
    connect(Remove_Point, QPushButton::clicked, this, Remove_Point_Clicked);
}


/**
 * @brief get the name of this editor.
 * @return name
 * @deprecated
 */
QString CommandEditor::getName(){
    return name;
}


/**
 * @brief set the name of this editor
 * @param newName
 * @deprecated
 */
void CommandEditor::setName(QString newName){
    this->name = newName;
    QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();
    lineEdits.at(0)->setText(this->name);
}




/**
 * @brief This method allows this class to know what the project name is that it is within.
 * @param name
 */
void CommandEditor::setProjectName(QString name){

    projectName = name;
}

/**
 * @brief sets whether this commandEditor is in "add command" mode or "save" mode.
 * Part of TEMP SOLUTION
 * @param bool for toggle
 */
void CommandEditor::setCommandAdded(bool commandAdded){
    this->commandAdded = commandAdded;
    if(commandAdded){
        this->Add_Command->setText("Save");
        this->Command_Name->setDisabled(true);
    }else{
        this->Add_Command->setText("Add Command");
        this->Command_Name->setEnabled(true);
    }
}

/**
 * @brief allows you to tell the command to add itself to the list from somewhere else.
 */
void CommandEditor::add_Command_Externally(){
    this->Add_Command_Clicked();
}

/**
 * @brief sets the point at the given index to the given coordinates.
 * @param index
 * @param x
 * @param y
 */
void CommandEditor::set_Point_At(int index, int x, int y){
    QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();
    lineEdits.at(index)->setText(QString::number(x) + "," + QString::number(y));
}

