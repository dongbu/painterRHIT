#include "stdafx.h"
#include "workspacewizard.h"

using namespace Ec;

WorkspaceWizard::WorkspaceWizard(QWidget *parent) :
QWidget(parent),
ui(new Ui::WorkspaceWizard)
{
	ui->setupUi(this);
	s1 = s2 = s3 = s4 = s5 = s6 = s7 = 0;
	stage = 0;
	numOfColors = 0;
	workspaceName = "Desktop/Workspace";
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

	//start off with empty variables in place
	canvasCorners.push_back(cv::Point3d(0, 0, 0));
	canvasCorners.push_back(cv::Point3d(0, 0, 0));
	canvasCorners.push_back(cv::Point3d(0, 0, 0));

	//connect buttons
	connect(ui->BackButton, SIGNAL(clicked()), this, SLOT(backPressed()));
	connect(ui->ForwardButton, SIGNAL(clicked()), this, SLOT(forwardPressed()));
	connect(browse, SIGNAL(clicked()), this, SLOT(browsePressed()));
	connect(ui->QuestionButton, SIGNAL(clicked()), this, SLOT(helpPressed()));

}

WorkspaceWizard::~WorkspaceWizard()
{
	delete ui;
}


void WorkspaceWizard::keyPressEvent(QKeyEvent *event){
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

void WorkspaceWizard::backPressed(){
	if (stage == 0){
		this->close();
	}
	stage--;
	updateText();
}

void WorkspaceWizard::forwardPressed(){
	stage++;
	updateText();
}

void WorkspaceWizard::updateText(){
	QString labelString;
	switch (stage){
	case 0:
		ui->BackButton->setText("Cancel");
		ui->Title->setText("Welcome to the Workspace Wizard");
		ui->Directions->setText("Press \"Next\" to continue");
		ui->QuestionButton->hide();
		ui->ForwardButton->setEnabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(false);
		break;
	case 1:
		ui->BackButton->setText("Back");
		ui->Title->setText("Setup Start Position");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u','j', 'm', and 'n' to put the robot into a stable position \npress 'e' when finished");
		ui->QuestionButton->show();
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 2:
		ui->Title->setText("Setup Top Left Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top left corner of the canvas \norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 3:
		ui->Title->setText("Setup Top Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top right corner of the canvas\norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 4:
		ui->Title->setText("Setup Bottom Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the bottom right corner of the canvas\norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		paintCountLabel->setText("");
		scaleSlide->setVisible(true);
		break;
	case 5:
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
	case 6:
		ui->Title->setText("Finish");
		ui->Directions->setText("All Done!\nPut in a file name/location and press finish to load your brand new workspace!");
		ui->ForwardButton->setText("Finish");
		nameEdit->setVisible(true);
		browse->setVisible(true);
		ui->QuestionButton->hide();
		paintCountLabel->setText("Please fill out file path below");
		break;
	case 7:
		finishWizard();
		break;
	default:
		ui->Title->setText("ERROR");
		ui->Directions->setText("ERROR");
		break;
	}
}

/*
0: clockwise
1: counter-clockwise
*/
void WorkspaceWizard::rotateBase(int direction){
	EcRealVector currentJoints;
	getJointValues(currentJoints);
	if (direction == 0){
		currentJoints[0] += (double)scaleSlide->value()/1000.0;
	}
	else if (direction == 1){
		currentJoints[0] -= (double)scaleSlide->value()/1000.0;
	}
	else{
		return;
	}
	setJointValues(currentJoints);

}


/*
0: forward -y
1: left +x
2: backwards +y
3: right -x
4: up +z
5: down -z
*/
void WorkspaceWizard::moveDirection(int direction){

	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation actualCoord;
	getActualPlacement(actualEEPlacement);
	actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();

	EcVector trans = actualCoord.translation();
	double x = trans.x();
	double y = trans.y();
	double z = trans.z();
	switch (direction){
	case 0: y -= (double)scaleSlide->value()/1000.0; break;
	case 1: x += (double)scaleSlide->value()/1000.0; break;
	case 2: y += (double)scaleSlide->value()/1000.0; break;
	case 3: x -= (double)scaleSlide->value()/1000.0; break;
	case 4: z += (double)scaleSlide->value()/1000.0; break;
	case 5: z -= (double)scaleSlide->value()/1000.0; break;
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
}

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

	delete nameEdit;
	delete paintCountLabel;
	delete browse;
	delete scaleSlide;


}

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

void WorkspaceWizard::helpPressed(){
	QMessageBox box;
	box.setInformativeText("<img src='C:/Users/doughezj/Pictures/workspaceHelp.png'/>");
	if (box.exec()){
		printf("goodbye\n");
	}
}