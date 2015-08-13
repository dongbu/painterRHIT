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
	int commandsFinished;
	QTimer *timer;
	QTime startTime;
	int secondCount, minuteCount;
	bool minPrep;

	QDialog *colorForm;
	Ui::colorChange colorUi;


private slots:
    void moveUpClicked();
    void moveDownClicked();
    void deleteCommandClicked();
    void launchRightClick(QPoint p);
    void menuSort(QAction *a);
	void cellChange(int curRow, int curCol, int prevRow, int prevCol);

	void drawingStarted();
	void drawingPaused();
	void drawingCleared();
	void showTime();

	void colorChangeConfirmed();

public slots:
    void populate();
	void recieveRunColor(int index, QString toggle);
    void recieveClearRunColors();

signals:
    void modifiedCommand();
	void highlightShape(int index);
    void runFrom(int index);
    void runOnly(int index);
    void setBreakPoint(int index);
};

#endif // COMMANDWINDOW_H