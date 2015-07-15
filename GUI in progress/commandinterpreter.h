#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H

#include "CytonController.h"
#include "DrawWindow.cpp"

#include <QListWidget>
#include <QTimer>
#include <vector>

class CommandInterpreter : public QObject
{
    Q_OBJECT

public:
	std::vector<int> breakPointList;

    CommandInterpreter();
	void beginPaintingCommands(int startIndex, int finishIndex);
    void stopPaintingCommands();
	void stepForwardCommands(int finishIndex);
    void stepBackwardCommands();
    void pausePaintingCommands();
    void clear();
	void setList(QListWidget *list);
	void setProjectLocation(QString projectLocation);

	int commandIndex, finishIndex, runFromAdjust, lineAttributeIndex, lineIndex, solidIndex;
	QString projectLocation;


private slots:
    void SendNext();

public slots:
    void beginConnecting(QString robot);

private:
	//General Variables
    DrawWindow *picasso;
	CytonController *bender;
	QTimer updateTimer;
	QListWidget *list;
	//General Variables

	//CommandList Variables
	QList<QString *> listOfCommandTypes;
	QString CurrentCommandType;

	std::vector<int> x, y, lineWidth; //line
	std::vector<QString> lineColor, lineStyle; //line
	//??			//Solid vars.
	//??			//Pixel vars.
	//CommandList Variables

	bool finished, paused, stopped, connected, prevContinuous, continuePainting;
	void BuildCommands();
	void ResetIndicies();
	void drawUntilCommand(int stopIndex);
	void MakeLine(QString commandName);
	void MakeSolid(QString commandName);
	void sendLine();
	void sendSolid();
	void setPaintbrush(QString colorHex);

signals:
	void tell_go_home(int pos);
	void send_Pos(double x1, double y1, double x2, double y2, bool continuous, bool prevContinuous, int currentEditor);
};

#endif // COMMANDINTERPRETER_H
