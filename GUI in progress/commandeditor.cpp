#include "commandeditor.h"
#include "guiloadsave.h"
#include <QTextStream>

CommandEditor::CommandEditor(QListWidget *widget) {
    PointVec = new std::vector<QLineEdit*>();
    pointCount = 0;
    this->BuildEditor();
    this->ConnectButtons();
    this->name = "untitled";
    this->widget = widget;
}

/*
 *This method is designed to create the CommandEditor widget
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

/*
 *This method is designed to create a std. point box, add it to
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

/*
 *This method is designed to populate a provided layout with the 5
 * input types necessary for controlling the command editor.
 */
void CommandEditor::PopulateParameters(QFormLayout *ParameterHolder) {
    //creating input lines
    Command_Name = new QLineEdit();
    Line_Color = new QComboBox();
    Line_Style = new QComboBox();

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
    ParameterHolder->addRow(new QLabel("<span style=\"text-align:center; font-size:40px \"> Command Editor </span> "));
    ParameterHolder->addRow("Command Name: ",Command_Name);
    ParameterHolder->addRow("Color: ",Line_Color);
    ParameterHolder->addRow("Style: ",Line_Style);
    MakePoint(); //adding 1st default point
    MakePoint(); //adding 2nd default point

}

/*
 *This method is designed to populate a provided layout with the 3
 * buttons necessary for controlling the command editor.
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

/*
 * Add_Command slot
 */
void CommandEditor::Add_Command_Clicked() {
    QTextStream(stdout) << "The add command button was pressed.\n";
    if(projectName.isEmpty()){
        return;
    } else{
        GuiLoadSave::writeCommandToFolder(projectName,this->CommandEditorWidget,widget);
    }

}

/*
 * Add_Point slot.
 */
void CommandEditor::Add_Point_Clicked() {
    Remove_Point->setDisabled(false);
    MakePoint();
}

/*
 * Remove_Point slot.
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

/*
 *This method is designed to connect the 3 editor buttons to their slots.
 */
void CommandEditor::ConnectButtons() {
    //Connecting button signals/slots
    connect(Add_Command, QPushButton::clicked, this, Add_Command_Clicked);
    connect(Add_Point, QPushButton::clicked, this, Add_Point_Clicked);
    connect(Remove_Point, QPushButton::clicked, this, Remove_Point_Clicked);
}

/*
 * this method returns the name of this editor.
 */
QString CommandEditor::getName(){
    return name;
}

/*
 * this method sets the name of this editor.
 */
void CommandEditor::setName(QString newName){
    this->name = newName;
}

/*
 * This method allows this class to know what the project name is that it is within.
 */
void CommandEditor::setProjectName(QString name){
    projectName = name;
}

