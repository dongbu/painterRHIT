#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include "stdafx.h"
#include "CytonRunner.h"
#include "Shapes.cpp"

namespace Ui {
class CommandWindow;
}

class CommandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandWindow(Shapes *ss, QWidget *parent = 0);
    ~CommandWindow();
    Shapes *shapes;
    Ui::CommandWindow *ui;

private:
	std::vector<int> getColor(QString col);

	QTimer *timer;
	QTime startTime;
	QDialog *colorForm;
	QComboBox *modeBox;
	QSpinBox *delayTime;
	QLabel *delayTimeLabel;
	QAction *delayTimeLabel_action, *delayTime_action;
	Ui::colorChange colorUi;
	int secondCount, minuteCount, commandsFinished;
	bool minPrep;

private slots:
    void moveUpClicked();
    void moveDownClicked();
    void deleteCommandClicked();
    void launchRightClick(QPoint p);
    void menuSort(QAction *a);
	void cellChange(int curRow, int curCol, int prevRow, int prevCol);
	void updateMode();
	void disableModeSetting();
	void enableModeSetting();

	void drawingStarted();
	void drawingPaused();
	void drawingCleared();
	void showTime();
	void colorChangeConfirmed();

public slots:
    void populate();
	void updateCommandList(int index, QString toggle);

signals:
    void modifiedCommand();
	void highlightShape(int index);
    void runFrom(int index);
    void runOnly(int index);
	void modeUpdated(QString mode, int delay);
};

#endif // COMMANDWINDOW_H