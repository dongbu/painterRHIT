#ifndef GUILOADSAVE_H
#define GUILOADSAVE_H

#include "mainwindow.h"
#include "commandeditor.h"
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
    static boolean writeCommandToFolder(QString ProjectName, QWidget* CommandEditor, QListWidget* CommandList, boolean CommandAdded);
    static int createProjectDirectory(QString ProjectName);
    static int writeCommandListToFolder(QString ProjectName, QListWidget* CommandList);
    static int loadCommandListFromFolder(QString ProjectName, QListWidget* CommandList);
    static QString saveAsProject();
    static boolean copyAllFilesFrom(QString prevFolder, QString newFolder);
    static boolean loadExternalFile(QString projectName, QListWidget* CommandList);
    static std::vector<std::vector<int> > getPointVectors(QString projectName, QListWidget* CommandList);


};

#endif // GUILOADSAVE_H