#include "StartWindow.h"
#include "ui_StartWindow.h"
#include "Painter.h"

StartWindow::StartWindow(QWidget *parent) :
QWidget(parent),
ui(new Ui::StartWindow)
{
	ui->setupUi(this);

	maxLength = 800;

	ui->sketchRadio->setChecked(true);

	//all connections for start window
	connect(ui->width, SIGNAL(editingFinished()), this, SLOT(updateExample()));
	connect(ui->height, SIGNAL(editingFinished()), this, SLOT(updateExample()));
	connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadClicked()));
	connect(ui->newButton, SIGNAL(clicked()), this, SLOT(newClicked()));
	connect(ui->cameraRadio, SIGNAL(clicked()), this, SLOT(radioChanged()));
	connect(ui->imageRadio, SIGNAL(clicked()), this, SLOT(radioChanged()));
	connect(ui->sketchRadio, SIGNAL(clicked()), this, SLOT(radioChanged()));
	connect(ui->cannyCheck, SIGNAL(clicked()), this, SLOT(checkboxChanged()));
	connect(ui->kMeanCheck, SIGNAL(clicked()), this, SLOT(checkboxChanged()));

	//hiding things as appropriate
	ui->kMeanCheck->hide();
	ui->cannyCheck->hide();
	ui->CCEdit->hide();
	ui->CCLabel->hide();
	ui->MLLEdit->hide();
	ui->MLLLabel->hide();
	ui->TEdit->hide();
	ui->TLabel->hide();
	ui->MRSEdit->hide();
	ui->MRSLabel->hide();

	//force things to be appropriate
	ui->width->setValidator(new QIntValidator(10, maxLength, this));
	ui->height->setValidator(new QIntValidator(10, maxLength, this));
	ui->CCEdit->setValidator(new QIntValidator(1, 64, this));
	ui->MLLEdit->setValidator(new QIntValidator(1, 100, this));
	ui->TEdit->setValidator(new QIntValidator(1, 100, this));
	ui->MRSEdit->setValidator(new QIntValidator(1, 100, this));
}

StartWindow::~StartWindow()
{
	delete ui;
}


void StartWindow::updateExample(){
	int w = 10;
	int h = 10;
	if (ui->width->text() != ""){
		w = (ui->width->text().toInt() / 4);
		if (w < 10){
			w = 10;
		}
	}
	if (ui->height->text() != ""){
		h = (ui->height->text().toInt() / 4);
		if (h < 10){
			h = 10;
		}
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
		Painter *painter = new Painter();
		int w = 10;
		int h = 10;
		if (ui->width->text() != ""){
			w = (ui->width->text().toInt());
			if (w < 10){
				w = 10;
			}
		}
		if (ui->height->text() != ""){
			h = (ui->height->text().toInt());
			if (h < 10){
				h = 10;
			}
		}
		painter->setDimensions(w, h);
		painter->showGUI(true);
		painter->sketch->loadWebcamPicture();
		this->close();
	}
	else if (ui->sketchRadio->isChecked()){
		Painter *painter = new Painter();
		int w = 10;
		int h = 10;
		if (ui->width->text() != ""){
			w = (ui->width->text().toInt());
			if (w < 10){
				w = 10;
			}
		}
		if (ui->height->text() != ""){
			h = (ui->height->text().toInt());
			if (h < 10){
				h = 10;
			}
		}
		painter->setDimensions(w, h);
		painter->showGUI(true);
		this->close();
	}
	else if (ui->imageRadio->isChecked()){
		bool kmeans = ui->kMeanCheck->isChecked();
		bool canny = ui->cannyCheck->isChecked();
		if (!kmeans && !canny){
			return;
		}
		Painter *painter = new Painter();

		QFileDialog directory;
		QStringList filters;
		filters << "Images (*.png *.xpm *.jpg)";
		directory.setNameFilters(filters);
		if (directory.exec()){
			cv::Mat image = cv::imread(directory.selectedFiles().at(0).toStdString());
			int w = image.size().width;
			int h = image.size().height;
			if (w >= h){
				if (w > maxLength){
					double scale = 800.0 / ((double)(w));
					w = 800;
					h = h*scale;
				}
			}
			else{
				if (h > maxLength){
					double scale = 800.0 / ((double)(h));
					h = 800;
					w = w*scale;
				}
			}
			painter->setDimensions(w, h);
			painter->showGUI(false);
			if (kmeans){
				painter->sketch->loadPhotoKmeansClicked(directory.selectedFiles().at(0).toStdString());
			}
			if (canny){
				painter->sketch->loadPhotoCannyClicked(directory.selectedFiles().at(0).toStdString());
			}
			painter->showGUI(true);
			this->close();
		}
	}
}
void StartWindow::radioChanged(){
	if (ui->cameraRadio->isChecked()){
		if (ui->kMeanCheck->isChecked()){
			ui->CCEdit->show();
			ui->CCLabel->show();
			ui->MRSEdit->show();
			ui->MRSLabel->show();
		}
		if (ui->cannyCheck->isChecked()){
			ui->TEdit->show();
			ui->TLabel->show();
			ui->MLLEdit->show();
			ui->MLLLabel->show();
		}

		ui->example->show();
		ui->kMeanCheck->show();
		ui->cannyCheck->show();
		ui->aspectRatioLabel->show();
		ui->X->show();
		ui->height->show();
		ui->width->show();
	}
	else if (ui->sketchRadio->isChecked()){
		ui->CCEdit->hide();
		ui->CCLabel->hide();
		ui->MLLEdit->hide();
		ui->MLLLabel->hide();
		ui->TEdit->hide();
		ui->TLabel->hide();
		ui->MRSLabel->hide();
		ui->MRSEdit->hide();

		ui->example->show();
		ui->kMeanCheck->hide();
		ui->cannyCheck->hide();
		ui->aspectRatioLabel->show();
		ui->X->show();
		ui->height->show();
		ui->width->show();
	}
	else if (ui->imageRadio->isChecked()){
		if (ui->kMeanCheck->isChecked()){
			ui->CCEdit->show();
			ui->CCLabel->show();
			ui->MRSEdit->show();
			ui->MRSLabel->show();
		}
		if (ui->cannyCheck->isChecked()){
			ui->TEdit->show();
			ui->TLabel->show();
			ui->MLLEdit->show();
			ui->MLLLabel->show();
		}

		ui->example->hide();
		ui->kMeanCheck->show();
		ui->cannyCheck->show();
		ui->aspectRatioLabel->hide();
		ui->X->hide();
		ui->height->hide();
		ui->width->hide();

	}

}

void StartWindow::checkboxChanged(){
	if (ui->kMeanCheck->isChecked()){
		ui->CCEdit->show();
		ui->CCLabel->show();
		ui->MRSEdit->show();
		ui->MRSLabel->show();
	}
	else{
		ui->CCEdit->hide();
		ui->CCLabel->hide();
		ui->MRSEdit->hide();
		ui->MRSLabel->hide();

	}
	if (ui->cannyCheck->isChecked()){
		ui->TEdit->show();
		ui->TLabel->show();
		ui->MLLEdit->show();
		ui->MLLLabel->show();
	}
	else{
		ui->TEdit->hide();
		ui->TLabel->hide();
		ui->MLLEdit->hide();
		ui->MLLLabel->hide();
	}
}