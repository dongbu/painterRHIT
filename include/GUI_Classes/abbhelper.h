#ifndef ABBHELPER_H
#define ABBHELPER_H

#include "stdafx.h"
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

	bool colorUsed[8]; //true if using color in spot
	int colorR[8];
	int colorG[8];
	int colorB[8];
	int portNum;


private:
	Ui::ABBHelper *ui;
	void connectCheckBoxes();
	void setupSpinBoxes();

public slots:
	

private slots :
	void updateColors();
	void updateSpins(int val);
	void acceptPressed();
	void cancelPressed();

signals:
	void canceledABB();
	void acceptedABB();

};

#endif // ABBHELPER_H
