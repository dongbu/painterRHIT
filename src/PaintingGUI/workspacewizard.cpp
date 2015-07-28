#include "stdafx.h"
#include "workspacewizard.h"

using namespace Ec;

WorkspaceWizard::WorkspaceWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkspaceWizard)
{
    ui->setupUi(this);
	stage = 0;
	numOfColors = 0;
	ui->QuestionButton->hide();
	ui->Directions->setWordWrap(true);
	connect(ui->BackButton, SIGNAL(clicked()), this, SLOT(backPressed()));
	connect(ui->ForwardButton, SIGNAL(clicked()), this, SLOT(forwardPressed()));

	paintCountLabel = new QLabel("Number of Color Locations: 0");

	//start off with empty variables in place
	canvasCorners.push_back(cv::Point3d(0, 0, 0));
	canvasCorners.push_back(cv::Point3d(0, 0, 0));
	canvasCorners.push_back(cv::Point3d(0, 0, 0));

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
		}else if (pressedKey == "a"){
			moveDirection(1);
		}else if (pressedKey == "s"){
			moveDirection(2);
		}else if (pressedKey == "d"){
			moveDirection(3);
		}else if (pressedKey =="u"){
			moveDirection(4);
		}else if (pressedKey == "j"){
			moveDirection(5);
		}else if (pressedKey == "m"){
			rotateBase(0);
		}else if (pressedKey == "n"){
			rotateBase(1);
		}else if (pressedKey == "e"){
			ui->ForwardButton->setEnabled(true);
			if (stage == 5){
				numOfColors++;
				paintCountLabel->setText(QString("Number of Color Locations: " + QString::number(numOfColors)));
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
		break;
	case 1:
		ui->BackButton->setText("Back");
		ui->Title->setText("Setup Start Position");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u','j', 'm', and 'n' to put the robot into a stable position \npress 'e' when finished");
		ui->QuestionButton->show();
		ui->ForwardButton->setDisabled(true);
		break;
	case 2:
		ui->Title->setText("Setup Top Left Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top left corner of the canvas \norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		break;
	case 3:
		ui->Title->setText("Setup Top Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top right corner of the canvas\norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		break;
	case 4:
		ui->Title->setText("Setup Bottom Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the bottom right corner of the canvas\norientation does not matter\npress 'e' when finished");
		ui->ForwardButton->setDisabled(true);
		break;
	case 5:
		numOfColors = 0;
		ui->Title->setText("Setup Paint Locations");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to put the robot above the paint location\nheight and orientation do not matter\npress 'e' to continue to next color");
		ui->ItemLayout->addWidget(paintCountLabel);
		ui->ForwardButton->setText("Next");
		break;
	case 6:
		ui->Title->setText("Finish");
		ui->Directions->setText("All Done!  press finish to load your brand new workspace!");
		ui->ForwardButton->setText("Finish");
		break;
	case 7:
		finishWizard();
		break;
	default:
		ui->Title->setText("");
		ui->Directions->setText("");
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
		currentJoints[0] += 0.1;
	}else if (direction == 1){
		currentJoints[0] -= 0.1;
	}else{
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
	case 0: y -= 0.01; break;
	case 1: x += 0.01; break;
	case 2: y += 0.01; break;
	case 3: x -= 0.01; break;
	case 4: z += 0.01; break;
	case 5: z -= 0.01; break;
	}
	EcCoordinateSystemTransformation pose;
	pose.setTranslationX(x);
	pose.setTranslationY(y);
	pose.setTranslationZ(z);

	printf("x: %f, y: %f, z: %f\n", x, y, z);
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
	}else if (stage >= 2 && stage <= 4){
		getActualPlacement(actualEEPlacement);
		actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();
		EcVector trans = actualCoord.translation();
		double x = trans.x();
		double y = trans.y();
		double z = trans.z();

		canvasCorners.at(stage - 2) = cv::Point3d(x, y, z);
	}else if (stage == 5){
		if (numOfColors <= paint.size()){
			paint.clear();
		}
		getActualPlacement(actualEEPlacement);
		actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();
		EcVector trans = actualCoord.translation();
		double x = trans.x();
		double y = trans.y();

		paint.push_back(std::pair<int,cv::Point>(numOfColors, cv::Point(x, y)));
	}
}

void WorkspaceWizard::finishWizard(){
	close();
	printf("TODO: actually save all of the users had work instead of just saying that it is saved.\n");
}