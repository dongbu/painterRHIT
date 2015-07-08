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
    static bool writeCommandToFolder(QString ProjectName, QWidget* Line, QListWidget* CommandList, bool CommandAdded, QString commandType);
    static int createProjectDirectory(QString ProjectName);
    static int writeCommandListToFolder(QString ProjectName, QListWidget* CommandList);
    static int loadCommandListFromFolder(QString ProjectName, QListWidget* CommandList);
    static QString saveAsProject();
    static bool copyAllFilesFrom(QString prevFolder, QString newFolder);
    static bool loadExternalFile(QString projectName, QListWidget* CommandList);
};

#endif // GUILOADSAVE_H
