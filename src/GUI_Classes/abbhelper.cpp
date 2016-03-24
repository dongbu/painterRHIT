#include "abbhelper.h"
#include "ui_abbhelper.h"

ABBHelper::ABBHelper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABBHelper)
{
    ui->setupUi(this);
}

ABBHelper::~ABBHelper()
{
    delete ui;
}
