#ifndef WORKSPACEWIZARD_H
#define WORKSPACEWIZARD_H

#include "stdafx.h"
#include "CytonRunner.h"
#include "ui_workspacewizard.h"

namespace Ui {
class WorkspaceWizard;
}

class WorkspaceWizard : public QWidget
{
    Q_OBJECT

public:
    explicit WorkspaceWizard(CytonRunner *Ava, QWidget *parent = 0);
    ~WorkspaceWizard();

private:
    Ui::WorkspaceWizard *ui;
	int stage;
	double s1, s2, s3, s4, s5, s6, s7;
	QTableWidget *tab;
	std::vector<cv::Point3d> canvasCorners;
	QLabel *paintCountLabel;
	QLineEdit* nameEdit;
	std::string workspaceName;
	QPushButton* browse;
	QSlider* scaleSlide;
	CytonRunner* Ava;
	bool moving;

	QRadioButton corner1, corner2, corner3;
	
	void updateText();
	void rotateBase(int direction);
	void moveDirection(int direction);
	void saveInfo();
	void finishWizard();
	bool checkTable();


protected:
  virtual void  keyPressEvent(QKeyEvent *event);

private slots:
  void forwardPressed();
  void backPressed();
  void browsePressed();
  void helpPressed();
  void resetSamples();

};

#endif // WORKSPACEWIZARD_H
