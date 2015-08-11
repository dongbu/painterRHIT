#include "StartWindow.h"
#include "ui_StartWindow.h"
#include "Painter.h"

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

	//all connections for start window
	connect(ui->width, SIGNAL(editingFinished()), this, SLOT(updateExample()));
	connect(ui->height, SIGNAL(editingFinished()), this, SLOT(updateExample()));
	connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadClicked()));
	connect(ui->newButton, SIGNAL(clicked()), this, SLOT(newClicked()));
	connect(ui->cameraRadio, SIGNAL(clicked()), this, SLOT(radioChanged()));
	connect(ui->imageRadio, SIGNAL(clicked()), this, SLOT(radioChanged()));
	connect(ui->sketchRadio, SIGNAL(clicked()), this, SLOT(radioChanged()));

	//hiding things as appropriate
	ui->kMeanCheck->hide();
	ui->cannyCheck->hide();
	ui->cameraRadio->setChecked(true);
	ui->width->setValidator(new QIntValidator(1, 800, this));
	ui->height->setValidator(new QIntValidator(1, 800, this));
}

StartWindow::~StartWindow()
{
    delete ui;
}


void StartWindow::updateExample(){
	int w = 1;
	int h = 1;
	if (ui->width->text() != ""){
		w = (ui->width->text().toInt()/4);
	}
	if (ui->height->text() != ""){
		h = (ui->height->text().toInt()/4);
	}
	ui->example->setFixedHeight(h);
	ui->example->setFixedWidth(w);
}
void StartWindow::loadClicked(){
	Painter *painter = new Painter();
	painter->showGUI(false);
	if (painter->sketch->openClicked()){
		painter->showGUI(true);
		this->close();
	}
}
void StartWindow::newClicked(){
	if (ui->cameraRadio->isChecked()){
		printf("do something here\n");
	}
	else if (ui->sketchRadio->isChecked()){
		Painter *painter = new Painter();
		int w = 1;
		int h = 1;
		if (ui->width->text() != ""){
			w = (ui->width->text().toInt());
		}
		if (ui->height->text() != ""){
			h = (ui->height->text().toInt());
		}
		painter->setDimensions(w, h);
		painter->showGUI(true);
		this->close();
	}
	else if (ui->imageRadio->isChecked()){
		printf("do something here\n");
	}
}
void StartWindow::radioChanged(){
	if (ui->cameraRadio->isChecked()){
		ui->kMeanCheck->hide();
		ui->cannyCheck->hide();
		ui->aspectRatioLabel->show();
		ui->X->show();
		ui->height->show();
		ui->width->show();
	}
	else if (ui->sketchRadio->isChecked()){
		ui->kMeanCheck->hide();
		ui->cannyCheck->hide();
		ui->aspectRatioLabel->show();
		ui->X->show();
		ui->height->show();
		ui->width->show();
	}
	else if (ui->imageRadio->isChecked()){
		ui->kMeanCheck->show();
		ui->cannyCheck->show();
		ui->aspectRatioLabel->hide();
		ui->X->hide();
		ui->height->hide();
		ui->width->hide();

	}
	
}