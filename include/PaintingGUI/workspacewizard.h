#ifndef WORKSPACEWIZARD_H
#define WORKSPACEWIZARD_H

#include "stdafx.h"
#include "ui_workspacewizard.h"
#include "CytonRunner.h"

namespace Ui {
class WorkspaceWizard;
}

class WorkspaceWizard : public QWidget
{
    Q_OBJECT

public:
    explicit WorkspaceWizard(QWidget *parent = 0);
    ~WorkspaceWizard();

private:
    Ui::WorkspaceWizard *ui;
	int stage, numOfColors;
	double s1, s2, s3, s4, s5, s6, s7;
	std::vector<cv::Point3d> canvasCorners;
	std::vector<std::pair<int, cv::Point>> paint;
	QLabel *paintCountLabel;
	std::string defaultName;
	
	void updateText();
	void rotateBase(int direction);
	void moveDirection(int direction);
	void saveInfo();
	void finishWizard();

protected:
  virtual void  keyPressEvent(QKeyEvent *event);

private slots:
  void forwardPressed();
  void backPressed();
};

#endif // WORKSPACEWIZARD_H
