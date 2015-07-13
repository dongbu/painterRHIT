#ifndef SOLID_H
#define SOLID_H

#include "ui_Solid.h"
#include <QWidget>

namespace Ui {
class Solid;
}

class Solid : public QWidget
{
    Q_OBJECT

public:
    explicit Solid(QWidget *parent = 0);
    ~Solid();

private:
    Ui::Solid *ui;
};

#endif // SOLID_H
