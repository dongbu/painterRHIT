#ifndef ABBHELPER_H
#define ABBHELPER_H

#include "stdafx.h"
#include "shapes.cpp"
#include <QWidget>
#include <qcheckbox.h>


namespace Ui {
class ABBHelper;
}

class ABBHelper : public QWidget
{
    Q_OBJECT

public:
    explicit ABBHelper(QWidget *parent = 0);
    ~ABBHelper();

	int portNum;

	void setConfirmationPanel(Shapes *shape);


private:
	Ui::ABBHelper *ui;


private slots :
	void acceptPressed();
	void cancelPressed();

signals:
	void canceledABB();
	void acceptedABB();

};

#endif // ABBHELPER_H
