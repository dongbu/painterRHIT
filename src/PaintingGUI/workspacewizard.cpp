#include "workspacewizard.h"


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
0: forward
1: left
2: backwards
3: right
4: up
5: down
*/
void WorkspaceWizard::rotateBase(int direction){

}

/*
0: clockwise
1: counter-clockwise
*/
void WorkspaceWizard::moveDirection(int direction){

}