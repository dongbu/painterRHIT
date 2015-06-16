#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H

#include "painter.h";
#include "pairedcoordinates.h"
#include <QListWidget>


class CommandInterpreter
{
public:
    CommandInterpreter(QString projectName);

    void beginPaintingCommands(QListWidget* widget);
    void stopPaintingCommands(QListWidget* widget);
    void stepForwardCommands(QListWidget* widget);
    void stepBackwardCommands(QListWidget* widget);
    void setProjectName(QString name);

private:
    int currentCommand;
    Painter *picasso;
    QString projectName;
    bool continuePainting;

    void clearPainter();
    void drawUntilCommand(QListWidget* widget, int stopPos);
    QList<PairedCoordinates> getCommandCoords(QString commandName);
};

#endif // COMMANDINTERPRETER_H
