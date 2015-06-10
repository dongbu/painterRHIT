#ifndef GUILOADSAVE_H
#define GUILOADSAVE_H

#include "mainwindow.h"
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

class GuiLoadSave
{

public:
    GuiLoadSave();
    static boolean writeCommandToFolder(QString ProjectName, QWidget* CommandEditor, QListWidget* CommandList);
    static int createProjectDirectory(QString ProjectName);
    static int updateCommandEditor(QString fileName, QString ProjectName, QWidget* CommandEditor);

};

#endif // GUILOADSAVE_H
