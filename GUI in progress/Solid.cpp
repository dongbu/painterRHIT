#include "solid.h"
#include "ui_solid.h"

Solid::Solid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Solid)
{
    ui->setupUi(this);
}

Solid::~Solid()
{
    delete ui;
}
