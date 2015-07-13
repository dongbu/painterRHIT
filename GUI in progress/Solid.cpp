#include "solid.h"
#include "guiloadsave.h"
#include <QFormLayout>

Solid::Solid(QWidget *parent) :
    ui(new Ui::Solid)
{
	ui->setupUi(this);

	PointVec = new std::vector<QLineEdit*>();
	pointCount = 0;
	this->BuildEditor();
	this->name = "untitled";
	this->commandAdded = false;

	connect(this, SIGNAL(signal_Info_Changed()), this, SLOT(InfoChanged()));
	connect(Line_Color, SIGNAL(currentIndexChanged(int)), this, SLOT(InfoChanged()));
	connect(Fill_Style, SIGNAL(currentIndexChanged(int)), this, SLOT(InfoChanged()));
	connect(Line_Width, SIGNAL(valueChanged(int)), this, SLOT(InfoChanged()));

	Solid::ui->verticalLayout->addWidget(this->CommandEditorWidget);
	this->show();
}

Solid::~Solid()
{
    delete ui;
}

/**
* @brief creates the Line widget
* representative of this class and assign it to the appropriate
* class variable.
*/
void Solid::BuildEditor() {
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

	//hide yo kids, hide yo wife, hide everything
	Line_Color->setEnabled(false);
	Line_Width->setEnabled(false);
	Fill_Style->setEnabled(true);


	this->CommandEditorWidget = HolderWidg;

	this->move(0, 0);
}

/**
* @brief creates a std. point box, add it to
* the point vector, and return said point
*/
void Solid::MakePoint() {
	QLineEdit *point = new QLineEdit();
	point->setPlaceholderText("0,0");
	point->setFixedSize(75, 22);
	PointVec->push_back(point);
	pointCount++;
	QString label = ("Point " + QString::number(pointCount) + ":");
	ParameterHolder->addRow(new QLabel(label), point);
	connect(point, SIGNAL(textChanged(QString)), this, SLOT(InfoChanged()));
}

/**
* @brief populates a provided layout with the 6
* input types necessary for controlling the command editor.
* @param ParameterHolder
*/
void Solid::PopulateParameters(QFormLayout *ParameterHolder) {
	//creating input lines
	Command_Name = new QLineEdit();
	Line_Color = new QComboBox();
	Fill_Style = new QComboBox();
	Line_Width = new QSpinBox();

	//forcing command name to be acceptable. Previously, entering only a number as a command name caused crash upon load.
	QRegExp rx("^[A-Za-z][A-Za-z0-9]*");
	Command_Name->setValidator(new QRegExpValidator(rx, this));

	//constraining sizes
	Command_Name->setFixedSize(200, 22);
	Line_Color->setFixedSize(200, 22);
	Fill_Style->setFixedSize(200, 22);
	Line_Width->setFixedSize(200, 22);

	//ComboBox Options
	QStringList colors, styles;
	colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";
	styles << "Random" << "Scanning" << "Recursion Fill";
	Line_Color->addItems(colors);
	Fill_Style->addItems(styles);

	//SpinBox Options
	Line_Width->setFixedWidth(60);
	Line_Width->setMinimum(1);
	Line_Width->setMaximum(20);
	Line_Width->setSingleStep(1);
	Line_Width->setValue(4);

	//adding input lines
	ParameterHolder->addRow("Command Name: ", Command_Name);
	ParameterHolder->addRow("Color: ", Line_Color);
	ParameterHolder->addRow("Style: ", Fill_Style);
	ParameterHolder->addRow("Width", Line_Width);
	MakePoint(); //adding 1st default point
	MakePoint(); //adding 2nd default point

}


/**
* @brief populates a provided layout with the 2
* buttons necessary for controlling the command editor.
* @param ButtonHolder
*/
void Solid::PopulateButtons(QGridLayout *ButtonHolder) {
	//creating buttons
	Add_Command = new QPushButton("Add Command");

	//adding buttons
	ButtonHolder->addWidget(Add_Command, 0, 0);
}


/**
* @brief Add_Command slot
*/

void Solid::Add_Command_Clicked() {

	QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();


	//very bad if we let the user overwrite the index file.  In fact, if this occurs, it will load non-existant commands.
	//these commands then crash the program.
	if (lineEdits.at(0)->text().toLower() == "index"){
		QMessageBox alert;
		alert.setText("Alert");
		alert.setInformativeText("index is protected");
		if (alert.exec()){
			return;
		}
	}

	//make sure everything else is acceptable.
	if (workSpace->projectName.isEmpty() || workSpace->projectName.isNull()){
		//creates a "Temp" folder to put things into until saved.
		if (!QDir("ProjectFiles/Temp").exists()){
			QDir().mkdir("ProjectFiles/Temp");
		}
		workSpace->projectName = "Temp";
	}
	lineEdits.at(0)->setDisabled(true);

	Solid::removeExcessLines();
	GuiLoadSave::writeCommandToFolder(workSpace->projectLocation, this->CommandEditorWidget, workSpace->list, commandAdded, "Line");
	commandAdded = true;
	//sets the name and changes the tabname
	Solid::setName(lineEdits.at(0)->text());
	emit fileStatusChanged();
	emit tell_Command_Added(-10);

	Solid::removeExcessLines();

	this->Add_Command->setText("Save");
	this->close();
}

void Solid::removeExcessLines(){
	//removes all empty lines
	QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();

	QList<QLayoutItem *> listToDelete;
	int i = 0;
	int linesRemoved = 0;
	foreach(QLineEdit *line, lineEdits){
		if ((line->text() == "" || line->text() == ",") && pointCount > 2 && i >= 2){
			pointCount--;
			if (PointVec->size() > lineEdits.indexOf(line) - 1){
				PointVec->erase(PointVec->begin() + lineEdits.indexOf(line) - 1);
			}
			int lastInput = lineEdits.indexOf(line) * 2 + 4 - linesRemoved;
			QLayoutItem *toDelete1 = ParameterHolder->itemAt(lastInput);
			listToDelete.append(toDelete1);
			lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();
			linesRemoved += 2;
		}
		i++;
	}
	foreach(QLayoutItem *item, listToDelete){
		ParameterHolder->removeItem(item);
		item->widget()->setVisible(false);
		item->widget()->deleteLater();
	}
}

/**
* @brief This method is designed to connect the 2 editor buttons to their slots.
*/
void Solid::ConnectButtons() {
	//Connecting button signals/slots
	connect(Add_Command, SIGNAL(clicked()), this, SLOT(Add_Command_Clicked()));
}


/**
* @brief get the name of this editor.
* @return name
* @deprecated
*/
QString Solid::getName(){
	return name;
}


/**
* @brief set the name of this editor
* @param newName
* @deprecated
*/
void Solid::setName(QString newName){
	this->name = newName;
	QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();
	lineEdits.at(0)->setText(this->name);
}

/**
* @brief sets whether this Line is in "add command" mode or "save" mode.
* Part of TEMP SOLUTION
* @param bool for toggle
*/
void Solid::setCommandAdded(bool commandAdded){
	this->commandAdded = commandAdded;
	if (commandAdded){
		this->Add_Command->setText("Save");
		this->Command_Name->setDisabled(true);
	}
	else{
		this->Add_Command->setText("Add Command");
		this->Command_Name->setEnabled(true);
	}
}

/**
* @brief allows you to tell the command to add itself to the list from somewhere else.
*/
void Solid::add_Command_Externally(QString projectName){
	workSpace->projectName = projectName;
	this->Add_Command_Clicked();
}

/**
* @brief sets the point at the given index to the given coordinates.
* @param index
* @param x
* @param y
*/
void Solid::set_Point_At(int index, int x, int y){
	QList<QLineEdit *> lineEdits = this->CommandEditorWidget->findChildren<QLineEdit *>();
	if (index > 1){
		lineEdits.at(index)->setText(QString::number(x) + "," + QString::number(y));
	}
}

void Solid::InfoChanged(){
	emit Solid::sendUpdateToDrawOn(this);
}

void Solid::updateLineStyles(QString color, QString style, int width){
	QList<QComboBox *> comboBoxes = this->CommandEditorWidget->findChildren<QComboBox *>();
	comboBoxes.at(0)->setCurrentText(color);
	comboBoxes.at(1)->setCurrentText(style);
	QList<QSpinBox *> spinBoxes = this->CommandEditorWidget->findChildren<QSpinBox *>();
	spinBoxes.at(0)->setValue(width);
}

void Solid::setWorkSpace(WorkSpace *workSpace){
	this->workSpace = workSpace;

}
