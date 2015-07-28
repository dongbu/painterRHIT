#ifndef WORKSPACEWIZARD_H
#define WORKSPACEWIZARD_H

#include "ui_workspacewizard.h"
#include "CytonRunner.h"
#include <QWidget>
#include <QKeyEvent>
#include <qstring.h>

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
	int stage;
	
	void updateText();
	void WorkspaceWizard::rotateBase(int direction);
	void WorkspaceWizard::moveDirection(int direction);

protected:
  virtual void  keyPressEvent(QKeyEvent *event);

private slots:
  void forwardPressed();
  void backPressed();
};

#endif // WORKSPACEWIZARD_H
