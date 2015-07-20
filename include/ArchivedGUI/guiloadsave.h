#ifndef GUILOADSAVE_H
#define GUILOADSAVE_H

#include "mainwindow.h"
#include "Line.h"
#include <QtGui>
#include <QMessageBox>
#include <QLineEdit>
#include <QList>
#include <QComboBox>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <iostream>
#include <QListWidget>
#include <vector>
#include <QDebug>

class GuiLoadSave
{

public:
    GuiLoadSave();
    static bool writeCommandToFolder(QString ProjectLocation, QWidget* Line, QListWidget* CommandList, bool CommandAdded, QString commandType);
    static int createProjectDirectory(QString ProjectLocation);
    static int writeCommandListToFolder(QString ProjectLocation, QListWidget* CommandList);
    static int loadCommandListFromFolder(QString ProjectLocation, QListWidget* CommandList);
    static QString saveAsProject(QString saveLocation);
    static bool copyAllFilesFrom(QString prevFolder, QString newFolder);
    static bool loadExternalFile(QString projectLocation, QListWidget* CommandList);
};

#endif // GUILOADSAVE_H
