#include "abbhelper.h"
#include "ui_abbhelper.h"

ABBHelper::ABBHelper(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ABBHelper)
{
	ui->setupUi(this);
	connectCheckBoxes();
	setupSpinBoxes();

	//disable ok button until something changes
	ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

	//connect two buttons on bottom
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptPressed()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelPressed()));
}

ABBHelper::~ABBHelper()
{
	delete ui;
}


//connect the checkboxes to disable/enable things as needed
void ABBHelper::connectCheckBoxes() {
	connect(ui->checkBox_c1, SIGNAL(stateChanged(int)), this, SLOT(updateSpins(int)));
	connect(ui->checkBox_c2, SIGNAL(stateChanged(int)), this, SLOT(updateSpins(int)));
	connect(ui->checkBox_c3, SIGNAL(stateChanged(int)), this, SLOT(updateSpins(int)));
	connect(ui->checkBox_c4, SIGNAL(stateChanged(int)), this, SLOT(updateSpins(int)));
	connect(ui->checkBox_c5, SIGNAL(stateChanged(int)), this, SLOT(updateSpins(int)));
	connect(ui->checkBox_c6, SIGNAL(stateChanged(int)), this, SLOT(updateSpins(int)));
}

//connect the spinboxes to update samples
void ABBHelper::setupSpinBoxes() {
	//color 1
	connect(ui->spinBox_c1R, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c1G, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c1B, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	//color 2
	connect(ui->spinBox_c2R, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c2G, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c2B, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	//color 3
	connect(ui->spinBox_c3R, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c3G, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c3B, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	//color 4
	connect(ui->spinBox_c4R, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c4G, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c4B, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	//color 5
	connect(ui->spinBox_c5R, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c5G, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c5B, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	//color 6
	connect(ui->spinBox_c6R, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c6G, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));
	connect(ui->spinBox_c6B, SIGNAL(valueChanged(int)), this, SLOT(updateColors()));

}

//update the spinBoxes to be enabled/disabled based on check boxes
void ABBHelper::updateSpins(int val) {

	bool enable1 = ui->checkBox_c1->isChecked();
	ui->label_c1Sample->setEnabled(enable1);
	ui->spinBox_c1R->setEnabled(enable1);
	ui->spinBox_c1G->setEnabled(enable1);
	ui->spinBox_c1B->setEnabled(enable1);

	bool enable2 = ui->checkBox_c2->isChecked();
	ui->label_c2Sample->setEnabled(enable2);
	ui->spinBox_c2R->setEnabled(enable2);
	ui->spinBox_c2G->setEnabled(enable2);
	ui->spinBox_c2B->setEnabled(enable2);

	bool enable3 = ui->checkBox_c3->isChecked();
	ui->label_c3Sample->setEnabled(enable3);
	ui->spinBox_c3R->setEnabled(enable3);
	ui->spinBox_c3G->setEnabled(enable3);
	ui->spinBox_c3B->setEnabled(enable3);

	bool enable4 = ui->checkBox_c4->isChecked();
	ui->label_c4Sample->setEnabled(enable4);
	ui->spinBox_c4R->setEnabled(enable4);
	ui->spinBox_c4G->setEnabled(enable4);
	ui->spinBox_c4B->setEnabled(enable4);

	bool enable5 = ui->checkBox_c5->isChecked();
	ui->label_c5Sample->setEnabled(enable5);
	ui->spinBox_c5R->setEnabled(enable5);
	ui->spinBox_c5G->setEnabled(enable5);
	ui->spinBox_c5B->setEnabled(enable5);

	bool enable6 = ui->checkBox_c6->isChecked();
	ui->label_c6Sample->setEnabled(enable6);
	ui->spinBox_c6R->setEnabled(enable6);
	ui->spinBox_c6G->setEnabled(enable6);
	ui->spinBox_c6B->setEnabled(enable6);

	if (enable1 || enable2 || enable3 || enable4 || enable5 || enable6) {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
	}
}


void ABBHelper::updateColors() {
	//update c1
	ui->label_c1Sample->setAutoFillBackground(true);
	QPalette c1Pal = ui->label_c1Sample->palette();
	int r1 = ui->spinBox_c1R->value();
	int g1 = ui->spinBox_c1G->value();
	int b1 = ui->spinBox_c1B->value();
	QColor c1Col = QColor(r1, g1, b1);
	c1Pal.setColor(QPalette::Window, c1Col);
	ui->label_c1Sample->setPalette(c1Pal);

	//update c2
	ui->label_c2Sample->setAutoFillBackground(true);
	QPalette c2Pal = ui->label_c2Sample->palette();
	int r2 = ui->spinBox_c2R->value();
	int g2 = ui->spinBox_c2G->value();
	int b2 = ui->spinBox_c2B->value();
	QColor c2Col = QColor(r2, g2, b2);
	c2Pal.setColor(QPalette::Window, c2Col);
	ui->label_c2Sample->setPalette(c2Pal);

	//update c3
	ui->label_c3Sample->setAutoFillBackground(true);
	QPalette c3Pal = ui->label_c3Sample->palette();
	int r3 = ui->spinBox_c3R->value();
	int g3 = ui->spinBox_c3G->value();
	int b3 = ui->spinBox_c3B->value();
	QColor c3Col = QColor(r3, g3, b3);
	c3Pal.setColor(QPalette::Window, c3Col);
	ui->label_c3Sample->setPalette(c3Pal);

	//update c4
	ui->label_c4Sample->setAutoFillBackground(true);
	QPalette c4Pal = ui->label_c4Sample->palette();
	int r4 = ui->spinBox_c4R->value();
	int g4 = ui->spinBox_c4G->value();
	int b4 = ui->spinBox_c4B->value();
	QColor c4Col = QColor(r4, g4, b4);
	c4Pal.setColor(QPalette::Window, c4Col);
	ui->label_c4Sample->setPalette(c4Pal);

	//update c5
	ui->label_c5Sample->setAutoFillBackground(true);
	QPalette c5Pal = ui->label_c5Sample->palette();
	int r5 = ui->spinBox_c5R->value();
	int g5 = ui->spinBox_c5G->value();
	int b5 = ui->spinBox_c5B->value();
	QColor c5Col = QColor(r5, g5, b5);
	c5Pal.setColor(QPalette::Window, c5Col);
	ui->label_c5Sample->setPalette(c5Pal);

	//update c6
	ui->label_c6Sample->setAutoFillBackground(true);
	QPalette c6Pal = ui->label_c6Sample->palette();
	int r6 = ui->spinBox_c6R->value();
	int g6 = ui->spinBox_c6G->value();
	int b6 = ui->spinBox_c6B->value();
	QColor c6Col = QColor(r6, g6, b6);
	c6Pal.setColor(QPalette::Window, c6Col);
	ui->label_c6Sample->setPalette(c6Pal);


}


void ABBHelper::acceptPressed() {
	this->close();
	this->portNum = ui->spinBox_comNum->value();

	//color 1
	this->colorUsed[0] = (ui->checkBox_c1->isChecked());
	if (colorUsed[0]) {
		this->colorR[0] = ui->spinBox_c1R->value();
		this->colorG[0] = ui->spinBox_c1G->value();
		this->colorB[0] = ui->spinBox_c1B->value();
	}
	//color 2
	this->colorUsed[1] = (ui->checkBox_c2->isChecked());
	if (colorUsed[1]) {
		this->colorR[1] = ui->spinBox_c2R->value();
		this->colorG[1] = ui->spinBox_c2G->value();
		this->colorB[1] = ui->spinBox_c2B->value();
	}
	//color 3
	this->colorUsed[2] = (ui->checkBox_c3->isChecked());
	if (colorUsed[2]) {
		this->colorR[2] = ui->spinBox_c3R->value();
		this->colorG[2] = ui->spinBox_c3G->value();
		this->colorB[2] = ui->spinBox_c3B->value();
	}
	//color 4
	this->colorUsed[3] = (ui->checkBox_c4->isChecked());
	if (colorUsed[3]) {
		this->colorR[3] = ui->spinBox_c4R->value();
		this->colorG[3] = ui->spinBox_c4G->value();
		this->colorB[3] = ui->spinBox_c4B->value();
	}
	//color 5
	this->colorUsed[4] = (ui->checkBox_c5->isChecked());
	if (colorUsed[4]) {
		this->colorR[4] = ui->spinBox_c5R->value();
		this->colorG[4] = ui->spinBox_c5G->value();
		this->colorB[4] = ui->spinBox_c5B->value();
	}
	//color 6
	this->colorUsed[5] = (ui->checkBox_c6->isChecked());
	if (colorUsed[5]) {
		this->colorR[5] = ui->spinBox_c6R->value();
		this->colorG[5] = ui->spinBox_c6G->value();
		this->colorB[5] = ui->spinBox_c6B->value();
	}
	
	//done with colors, tell ABBRunner
	emit acceptedABB();
}


void ABBHelper::cancelPressed() {
	emit canceledABB();
	this->close();
}