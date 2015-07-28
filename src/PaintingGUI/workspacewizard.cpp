#include "workspacewizard.h"

using namespace Ec;

WorkspaceWizard::WorkspaceWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkspaceWizard)
{
	printf("making\n");
    ui->setupUi(this);
	stage = 0;
	ui->QuestionButton->hide();
	//ui->Directions->setFixedSize(ui->Directions->size());
	ui->Directions->setWordWrap(true);
	connect(ui->BackButton, SIGNAL(clicked()), this, SLOT(backPressed()));
	connect(ui->ForwardButton, SIGNAL(clicked()), this, SLOT(forwardPressed()));
}

WorkspaceWizard::~WorkspaceWizard()
{
    delete ui;
}


void WorkspaceWizard::keyPressEvent(QKeyEvent *event){
	if (stage == 1 || stage == 2 || stage == 3 || stage == 4){
		QString pressedKey = event->text();
		if (pressedKey == "w"){
			printf("move forward\n");
			moveDirection(0);
		}else if (pressedKey == "a"){
			printf("move left\n");
			moveDirection(1);
		}else if (pressedKey == "s"){
			printf("move backwards\n");
			moveDirection(2);
		}else if (pressedKey == "d"){
			printf("move right\n");
			moveDirection(3);
		}else if (pressedKey == "u"){
			printf("move up\n");
			moveDirection(4);
		}else if (pressedKey == "j"){
			printf("move down\n");
			moveDirection(5);
		}else if (pressedKey == "m"){
			printf("rotate clockwise\n");
			rotateBase(0);
		}else if (pressedKey == "n"){
			printf("rotate counter-clockwise\n");
			rotateBase(1);
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
	switch (stage){
	case 0: 
		ui->BackButton->setText("Cancel");
		ui->Title->setText("Welcome to the Workspace Wizard");
		ui->Directions->setText("Press \"Next\" to continue");
		ui->QuestionButton->hide();
		break;
	case 1:
		ui->BackButton->setText("Back");
		ui->Title->setText("Setup Home Position");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u','j', 'm', and 'n' to put the robot into a stable position press \"next\" when finished");
		ui->QuestionButton->show();
		break;
	case 2:
		ui->Title->setText("Setup Top Left Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top left corner of the canvas press \"next\" when finished");
		break;
	case 3:
		ui->Title->setText("Setup Top Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the top right corner of the canvas press \"next\" when finished");
		break;
	case 4:
		ui->Title->setText("Setup Bottom Right Canvas Corner");
		ui->Directions->setText("use keys 'w', 'a', 's', 'd', 'u', 'j', 'm', and 'n' to touch the robot gripper to the bottom right corner of the canvas press \"next\" when finished");
		break;
	default:
		ui->Title->setText("");
		ui->Directions->setText("");
		ui->ItemFrame->hide();
		break;
	}
}

/*
0: clockwise
1: counter-clockwise
*/
void WorkspaceWizard::rotateBase(int direction){
	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation offset, zero, actualCoord;
	getActualPlacement(actualEEPlacement);
	actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();

	
	
}


/*
0: forward + y
1: left -x
2: backwards -y
3: right +x
4: up +z
5: down -z
*/
void WorkspaceWizard::moveDirection(int direction){
	
	EcManipulatorEndEffectorPlacement actualEEPlacement;
	EcCoordinateSystemTransformation offset, zero, actualCoord;
	getActualPlacement(actualEEPlacement);
	actualCoord = actualEEPlacement.offsetTransformations()[0].coordSysXForm();

	EcVector trans = actualCoord.translation();
	double x = trans.x();
	double y = trans.y();
	double z = trans.z();
	switch (direction){
	case 0:
		y += 0.01;
		break;
	case 1:
		x -= 0.01;
		break;
	case 2:
		y -= 0.01;
		break;
	case 3:
		x += 0.01;
		break;
	case 4:
		z += 0.01;
		break;
	case 5:
		z -= 0.01;
		break;
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