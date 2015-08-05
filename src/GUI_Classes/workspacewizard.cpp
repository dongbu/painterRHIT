#include "stdafx.h"
#include "workspacewizard.h"
#include "CytonRunner.h"
#include "windows.h"

using namespace Ec;

/*
* Constructor for window that can be used to create/setup a workspace.
*/
WorkspaceWizard::WorkspaceWizard(CytonRunner *Ava, QWidget *parent) :
QWidget(parent),
ui(new Ui::WorkspaceWizard)
{
	ui->setupUi(this);
	this->setWindowTitle("Workspace Wizard");
	//starting rotation points
	s1 = s2 = s3 = s4 = s5 = s6 = s7 = 0;

	stage = 0;
	numOfColors = 0;
	workspaceName = "Desktop/Workspace";
	moving = false;

	//ui setup
	ui->QuestionButton->hide();
	ui->QuestionButton->setToolTip("help");
	ui->Directions->setWordWrap(true);
	paintCountLabel = new QLabel("");
	nameEdit = new QLineEdit();
	nameEdit->setClearButtonEnabled(true);
	ui->ItemLayout->addWidget(paintCountLabel);
	ui->ItemLayout->addWidget(nameEdit);
	nameEdit->setVisible(false);
	browse = new QPushButton("...");
	browse->setToolTip("Browse");
	ui->ItemLayout->addWidget(browse);
	ui->ItemLayout->addWidget(browse, 6, Qt::AlignRight);
	browse->setVisible(false);
	scaleSlide = new QSlider();
	scaleSlide->setOrientation(Qt::Horizontal);
	scaleSlide->setRange(1, 50);
	scaleSlide->setValue(10);
	scaleSlide->setVisible(false);
	ui->ItemLayout->addWidget(scaleSlide);

	//instance of the robot that is involved with this.
	this->Ava = Ava;

	//start off with empty variables in place
	canvasCorners.push_back(cv::Point3d(0, 0, 0));
	canvasCorners.push_back(cv::Point3d(0, 0, 0));
	canvasCorners.push_back(cv::Point3d(0, 0, 0));

	//connect buttons
	connect(ui->BackButton, SIGNAL(clicked()), this, SLOT(backPressed()));
	connect(ui->ForwardButton, SIGNAL(clicked()), this, SLOT(forwardPressed()));
	connect(browse, SIGNAL(clicked()), this, SLOT(browsePressed()));
	connect(ui->QuestionButton, SIGNAL(clicked()), this, SLOT(helpPressed()));


	forwardPressed(); //prevent "welcome" page from showing.

}

/*
* Virtual deconstructor
*/
WorkspaceWizard::~WorkspaceWizard(){
	delete ui;
}

/*
recieves keypress input and performs the specified actions (if appropriate)
*/
void WorkspaceWizard::keyPressEvent(QKeyEvent *event){
	if (moving) return;
	Sleep(27);

	if (stage == 1 || stage == 2 || stage == 3 || stage == 4 || stage == 5){
		QString pressedKey = event->text();
		if (pressedKey == "w"){
			moveDirection(0);
		}
		else if (pressedKey == "a"){
			moveDirection(1);
		}
		else if (pressedKey == "s"){
			moveDirection(2);
		}
		else if (pressedKey == "d"){
			moveDirection(3);
		}
		else if (pressedKey == "u"){
			moveDirection(4);
		}
		else if (pressedKey == "j"){
			moveDirection(5);
		}
		else if (pressedKey == "m"){
			rotateBase(0);
		}
		else if (pressedKey == "n"){
			rotateBase(1);
		}
		else if (pressedKey == "e"){
			ui->ForwardButton->setEnabled(true);
			if (stage == 5){
				numOfColors++;
				paintCountLabel->setText(QString("Number of Color Locations: " + QString::number(numOfColors)));
			}
			else{
				paintCountLabel->setText("position saved!");
			}
			saveInfo();
		}
	}
}

/*
slot for "back" button.  closes if on the first window (stage 0)
*/
void WorkspaceWizard::backPressed(){
	if (stage == 1){
		this->close();
	}
	stage--;
	updateText();
}

/*
slot for the "Next" button.  closes and saves if on the last window (stage 6)
*/
void WorkspaceWizard::forwardPressed(){
	stage++;
	updateText();
}

/*
updates the label text and button text etc, of the main window.
Also hides and shows things as appropriate.
*/
void WorkspaceWizard::updateText(){
	QString labelString;
	switch (stage){
	case 1: //setup home position
		ui->BackButton->setText("Cancel");
		ui->Title->setText("Setup Start Position");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u','j', 'm', and 'n' to put the robot into a stable home position \npress 'e' when finished");
		ui->QuestionButton->show();
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 2: //setup canvas corner one
		ui->Title->setText("Setup Top Left Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top left corner of the canvas \norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 3: //setup canvas corner two
		ui->Title->setText("Setup Top Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top right corner of the canvas\norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 4: //setup canvas corner 3
		ui->Title->setText("Setup Bottom Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the bottom right corner of the canvas\norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 5: //setup as many paint locations as necessary.
		numOfColors = 0;
		ui->Title->setText("Setup Paint Locations");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to put the robot gripper tip above the paint location\nheight and orientation do not matter\npress 'e' to continue to next color");
		paintCountLabel->setText("Number of Color Locations: 0");
		ui->ForwardButton->setText("Next");
		nameEdit->setVisible(false);
		browse->setVisible(false);
		ui->QuestionButton->show();
		scaleSlide->setVisible(true);
		break;
	case 6: //finish it all off.
		ui->Title->setText("Finish");
		ui->Directions->setText("All Done!\nPut in a file name/location and press finish to load your brand new workspace!");
		ui->ForwardButton->setText("Finish");
		nameEdit->setVisible(true);
		browse->setVisible(true);
		ui->QuestionButton->hide();
		paintCountLabel->setText("Please fill out file path below");
		scaleSlide->setVisible(false);
		break;
	case 7: //things are finished, we need to save now.
		finishWizard();
		break;
	default: //should never hit default.
		ui->Title->setText("ERROR");
		ui->Directions->setText("ERROR");
		break;
	}
}

/*
rotates the base of the robot
0: clockwise
1: counter-clockwise
*/
void WorkspaceWizard::rotateBase(int direction){
	moving = true;

	EcRealVector currentJoints;
	getJointValues(currentJoints);
	if (direction == 0){
		currentJoints[0] += (double)scaleSlide->value() / 1000.0;
	}
	else if (direction == 1){
		currentJoints[0] -= (double)scaleSlide->value() / 1000.0;
	}
	else{
		return;
	}
	setJointValues(currentJoints);

	moving = false;
}


/*
//moves the robot gripper tip in the specified direction.
0: forward -y
1: left +x
2: backwards +y
3: right -x
4: up +z
5: down -z
*/
void WorkspaceWizard::moveDirection(int direction){
	moving = true;

	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation actualCoord;
	getActualPlacement(actualEEPlacement);
	actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();

	EcVector trans = actualCoord.translation();
	double x = trans.x();
	double y = trans.y();
	double z = trans.z();
	switch (direction){
	case 0: y -= (double)scaleSlide->value() / 1000.0; break;
	case 1: x += (double)scaleSlide->value() / 1000.0; break;
	case 2: y += (double)scaleSlide->value() / 1000.0; break;
	case 3: x -= (double)scaleSlide->value() / 1000.0; break;
	case 4: z += (double)scaleSlide->value() / 1000.0; break;
	case 5: z -= (double)scaleSlide->value() / 1000.0; break;
	}

	EcCoordinateSystemTransformation pose;
	pose.setTranslationX(x);
	pose.setTranslationY(y);
	pose.setTranslationZ(z);

	printf("x: %f, y: %f, z: %f moving at %f step speed\n", x, y, z, (double)scaleSlide->value() / 1000.0);

	EcOrientation orientation;
	orientation.setFrom123Euler(0, 0, 0);
	pose.setOrientation(orientation);
	setEndEffectorSet(0); //point end effector set

	EcEndEffectorPlacement desiredPlacement(pose);

	setDesiredPlacement(desiredPlacement, 0, 0);

	moving = false;
}

/*
obtains the necessary information from the robot
and stores them as field variables.
*/
void WorkspaceWizard::saveInfo(){
	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation actualCoord;

	if (stage == 1){
		EcRealVector currentJoints;
		getJointValues(currentJoints);
		s1 = currentJoints[0]; s2 = currentJoints[1]; s3 = currentJoints[2]; s4 = currentJoints[3];
		s5 = currentJoints[4]; s6 = currentJoints[5]; s7 = currentJoints[6];
	}
	else if (stage >= 2 && stage <= 4){
		getActualPlacement(actualEEPlacement);
		actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();
		EcVector trans = actualCoord.translation();
		double x = trans.x();
		double y = trans.y();
		double z = trans.z();

		canvasCorners.at(stage - 2) = cv::Point3d(x, y, z);
	}
	else if (stage == 5){
		if (numOfColors <= paint.size()){
			paint.clear();
		}
		getActualPlacement(actualEEPlacement);
		actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();
		EcVector trans = actualCoord.translation();
		double x = trans.x();
		double y = trans.y();

		paint.push_back(std::pair<int, cv::Point2d>(numOfColors, cv::Point2d(x, y)));
	}
}

/*
closes the window and saves all of the important information in an xml file.
Also loads said xml file.
*/
void WorkspaceWizard::finishWizard(){
	close();

	std::string line = "<?xml version=\"1.0\"?>\n";;
	line.append("<workspace>");
	line.append("<starting>");
	line.append(string_format("<rotation s=\"%f\"/>", s1));
	line.append(string_format("<rotation s=\"%f\"/>", s2));
	line.append(string_format("<rotation s=\"%f\"/>", s3));
	line.append(string_format("<rotation s=\"%f\"/>", s4));
	line.append(string_format("<rotation s=\"%f\"/>", s5));
	line.append(string_format("<rotation s=\"%f\"/>", s6));
	line.append(string_format("<rotation s=\"%f\"/>", s7));
	line.append("</starting>");
	line.append("<canvas>");
	for (size_t i = 0; i < canvasCorners.size(); i++){
		cv::Point3d p = canvasCorners.at(i);
		line.append(string_format("<corner x=\"%f\" y=\"%f\" z=\"%f\"/>", p.x, p.y, p.z));
	}
	line.append("</canvas>");
	line.append("<paintPickup>");
	for (size_t i = 0; i < paint.size(); i++){
		std::pair<int, cv::Point2d> p = paint.at(i);
		line.append(string_format("<point x=\"%f\" y=\"%f\" id=\"%d\"/>", p.second.x, p.second.y, p.first));
	}
	line.append("</paintPickup>");
	line.append("</workspace>");

	std::string saveName = nameEdit->text().toStdString();

	std::ofstream myfile;
	myfile.open(saveName);
	myfile << line;
	myfile.close();

	Ava->loadWorkspace(saveName);

	delete nameEdit;
	delete paintCountLabel;
	delete browse;
	delete scaleSlide;

	Ava->tellFinished();
}

/*
does the same thing here as everywhere else.
*/
// not a beautiful place for this but c'est la vie
std::string WorkspaceWizard::string_format(const std::string fmt, ...) {
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	std::string str;
	va_list ap;
	while (1) {     // Maximum two passes on a POSIX system...
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) { str.resize(n); return str; }
		if (n > -1) size = n + 1; else size *= 2;
	}
	return str;
}

/*
opens a file directory to select the name (and location) to save the new xml.
*/
void WorkspaceWizard::browsePressed(){
	QFileDialog directory;
	directory.setDirectory("Libraries/Documents");
	QStringList filters;
	filters << "XML files (*.xml)";
	directory.setNameFilters(filters);
	directory.setAcceptMode(QFileDialog::AcceptSave);
	directory.setConfirmOverwrite(false);
	if (directory.exec()){
		QString fileLoc = directory.selectedFiles().at(0);
		if (!fileLoc.split("/").last().contains(QRegExp("[" + QRegExp::escape("\\/:*?\"<>|") + "]"))){
			this->nameEdit->setText(fileLoc);
		}
		else{
			this->nameEdit->setText("INVALID SELECTION");
			printf("%s\n", fileLoc.toStdString().c_str());
		}
	}
}

/*
slot for help button.
shows a quick (and hastily made) diagram of how to use the keys to move the robot.
*/
void WorkspaceWizard::helpPressed(){
	QDialog d;
	QGraphicsScene scene;
	QGraphicsView view(&scene);
	QGraphicsPixmapItem item(QPixmap("workspaceHelp.png"));
	scene.addItem(&item);
	QVBoxLayout *l = new QVBoxLayout();
	l->addWidget(&view);
	d.setLayout(l);

	if (d.exec()){
		printf("goodbye again\n");
	}
}