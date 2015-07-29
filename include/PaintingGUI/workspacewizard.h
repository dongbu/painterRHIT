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
	std::vector<std::pair<int, cv::Point2d>> paint;
	QLabel *paintCountLabel;
	QLineEdit* nameEdit;
	std::string workspaceName;
	QPushButton* browse;
	QSlider* scaleSlide;
	
	void updateText();
	void rotateBase(int direction);
	void moveDirection(int direction);
	void saveInfo();
	void finishWizard();
	std::string string_format(const std::string fmt, ...);


protected:
  virtual void  keyPressEvent(QKeyEvent *event);

private slots:
  void forwardPressed();
  void backPressed();
  void browsePressed();
  void helpPressed();
};

#endif // WORKSPACEWIZARD_H
