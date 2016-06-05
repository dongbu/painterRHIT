#include "abbhelper.h"
#include "ui_abbhelper.h"

ABBHelper::ABBHelper(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ABBHelper)
{
	ui->setupUi(this);

	//connect two buttons on bottom
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptPressed()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelPressed()));
	
}

ABBHelper::~ABBHelper()
{
	delete ui;
}


void ABBHelper::acceptPressed() {
	this->close();
	this->portNum = ui->spinBox_comNum->value();
	
	//done with colors, tell ABBRunner
	emit acceptedABB();
}


void ABBHelper::cancelPressed() {
	emit canceledABB();
	this->close();
}

void ABBHelper::setConfirmationPanel(Shapes *shape) {
	std::vector<cv::Scalar> existingColors;
	for (int i = 0; i < shape->length(); i++) {
		std::cout << i;
		cv::Scalar penCol = shape->at(i)->getPenColor(); //color col
		bool alreadyExistingColor = 0;
		for (int j = 0; j < existingColors.size(); j++) {
			if (penCol[0] == existingColors.at(j)[0] && penCol[1] == existingColors.at(j)[1] && penCol[2] == existingColors.at(j)[2]) {
				alreadyExistingColor = 1;
				break;
			}
		}
		if (!alreadyExistingColor) {
			existingColors.push_back(penCol);
			QLabel *lab = new QLabel();
			lab->setText(QString(shape->at(i)->pos));
			ui->gridLayout->addWidget(lab, i + 1, 0);
			QLabel *lab2 = new QLabel();
			lab2->setAutoFillBackground(true); // IMPORTANT!
			QPalette pal = lab2->palette();
			pal.setColor(QPalette::Window, QColor(shape->at(i)->getPenColor()[0], shape->at(i)->getPenColor()[1], shape->at(i)->getPenColor()[2]));
			lab2->setPalette(pal);
			ui->gridLayout->addWidget(lab2, i + 1, 1);
			ui->gridLayout->update();
		}
	}
}