#ifndef ABBHELPER_H
#define ABBHELPER_H

#include <QWidget>

namespace Ui {
class ABBHelper;
}

class ABBHelper : public QWidget
{
    Q_OBJECT

public:
    explicit ABBHelper(QWidget *parent = 0);
    ~ABBHelper();

private:
    Ui::ABBHelper *ui;
};

#endif // ABBHELPER_H
