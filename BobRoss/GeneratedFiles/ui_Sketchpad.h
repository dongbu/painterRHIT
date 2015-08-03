/********************************************************************************
** Form generated from reading UI file 'Sketchpad.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SKETCHPAD_H
#define UI_SKETCHPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sketchpad
{
public:
    QAction *actionOpen;
    QAction *actionNew;
    QAction *actionDraw_Square;
    QAction *actionDraw_Circle;
    QAction *actionDraw_Line;
    QAction *actionSave_As;
    QAction *actionSave;
    QAction *actionDraw_Filled_Circle;
    QAction *actionDraw_Filled_Rectangle;
    QAction *actionDraw_Filled_Polygon;
    QAction *actionShutdown;
    QAction *actionLoad;
    QAction *actionCreate;
    QAction *actionActionFill;
    QAction *actionLoad_Photo;
    QAction *actionLaunch_webcam;
    QWidget *centralwidget;
    QWidget *widget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuRobot;
    QMenu *menuWorkspace;
    QMenu *menuImage;
    QStatusBar *statusbar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *Sketchpad)
    {
        if (Sketchpad->objectName().isEmpty())
            Sketchpad->setObjectName(QStringLiteral("Sketchpad"));
        Sketchpad->resize(924, 857);
        actionOpen = new QAction(Sketchpad);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon Storage/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionNew = new QAction(Sketchpad);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon Storage/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon1);
        actionDraw_Square = new QAction(Sketchpad);
        actionDraw_Square->setObjectName(QStringLiteral("actionDraw_Square"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Icon Storage/Square.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Square->setIcon(icon2);
        actionDraw_Circle = new QAction(Sketchpad);
        actionDraw_Circle->setObjectName(QStringLiteral("actionDraw_Circle"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Icon Storage/Circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Circle->setIcon(icon3);
        actionDraw_Line = new QAction(Sketchpad);
        actionDraw_Line->setObjectName(QStringLiteral("actionDraw_Line"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Icon Storage/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Line->setIcon(icon4);
        actionSave_As = new QAction(Sketchpad);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Icon Storage/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon5);
        actionSave = new QAction(Sketchpad);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave->setIcon(icon5);
        actionDraw_Filled_Circle = new QAction(Sketchpad);
        actionDraw_Filled_Circle->setObjectName(QStringLiteral("actionDraw_Filled_Circle"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Icon Storage/FilledCircle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Filled_Circle->setIcon(icon6);
        actionDraw_Filled_Rectangle = new QAction(Sketchpad);
        actionDraw_Filled_Rectangle->setObjectName(QStringLiteral("actionDraw_Filled_Rectangle"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Icon Storage/FilledSquare.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Filled_Rectangle->setIcon(icon7);
        actionDraw_Filled_Polygon = new QAction(Sketchpad);
        actionDraw_Filled_Polygon->setObjectName(QStringLiteral("actionDraw_Filled_Polygon"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/Icon Storage/FreeForm.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Filled_Polygon->setIcon(icon8);
        actionShutdown = new QAction(Sketchpad);
        actionShutdown->setObjectName(QStringLiteral("actionShutdown"));
        actionLoad = new QAction(Sketchpad);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionCreate = new QAction(Sketchpad);
        actionCreate->setObjectName(QStringLiteral("actionCreate"));
        actionActionFill = new QAction(Sketchpad);
        actionActionFill->setObjectName(QStringLiteral("actionActionFill"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/Icon Storage/fill.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionActionFill->setIcon(icon9);
        actionLoad_Photo = new QAction(Sketchpad);
        actionLoad_Photo->setObjectName(QStringLiteral("actionLoad_Photo"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/Icon Storage/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Photo->setIcon(icon10);
        actionLaunch_webcam = new QAction(Sketchpad);
        actionLaunch_webcam->setObjectName(QStringLiteral("actionLaunch_webcam"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/Icon Storage/webcam.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLaunch_webcam->setIcon(icon11);
        centralwidget = new QWidget(Sketchpad);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 901, 750));
        widget->setCursor(QCursor(Qt::CrossCursor));
        Sketchpad->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Sketchpad);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 924, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRobot = new QMenu(menubar);
        menuRobot->setObjectName(QStringLiteral("menuRobot"));
        menuWorkspace = new QMenu(menuRobot);
        menuWorkspace->setObjectName(QStringLiteral("menuWorkspace"));
        menuImage = new QMenu(menubar);
        menuImage->setObjectName(QStringLiteral("menuImage"));
        Sketchpad->setMenuBar(menubar);
        statusbar = new QStatusBar(Sketchpad);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Sketchpad->setStatusBar(statusbar);
        toolBar_2 = new QToolBar(Sketchpad);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        Sketchpad->addToolBar(Qt::TopToolBarArea, toolBar_2);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuRobot->menuAction());
        menubar->addAction(menuImage->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuRobot->addAction(menuWorkspace->menuAction());
        menuRobot->addAction(actionShutdown);
        menuWorkspace->addAction(actionCreate);
        menuWorkspace->addAction(actionLoad);
        menuImage->addAction(actionLoad_Photo);
        menuImage->addAction(actionLaunch_webcam);
        toolBar_2->addAction(actionActionFill);
        toolBar_2->addAction(actionDraw_Filled_Rectangle);
        toolBar_2->addAction(actionDraw_Filled_Circle);
        toolBar_2->addSeparator();
        toolBar_2->addAction(actionDraw_Line);
        toolBar_2->addSeparator();
        toolBar_2->addAction(actionDraw_Circle);
        toolBar_2->addAction(actionDraw_Square);
        toolBar_2->addAction(actionDraw_Filled_Polygon);

        retranslateUi(Sketchpad);

        QMetaObject::connectSlotsByName(Sketchpad);
    } // setupUi

    void retranslateUi(QMainWindow *Sketchpad)
    {
        Sketchpad->setWindowTitle(QApplication::translate("Sketchpad", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("Sketchpad", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("Sketchpad", "Ctrl+O", 0));
        actionNew->setText(QApplication::translate("Sketchpad", "New", 0));
        actionNew->setShortcut(QApplication::translate("Sketchpad", "Ctrl+N", 0));
        actionDraw_Square->setText(QApplication::translate("Sketchpad", "Draw Square", 0));
        actionDraw_Circle->setText(QApplication::translate("Sketchpad", "Draw Circle", 0));
        actionDraw_Line->setText(QApplication::translate("Sketchpad", "Draw Line", 0));
        actionSave_As->setText(QApplication::translate("Sketchpad", "Save As", 0));
        actionSave->setText(QApplication::translate("Sketchpad", "Save", 0));
        actionSave->setShortcut(QApplication::translate("Sketchpad", "Ctrl+S", 0));
        actionDraw_Filled_Circle->setText(QApplication::translate("Sketchpad", "Draw Filled Circle", 0));
        actionDraw_Filled_Rectangle->setText(QApplication::translate("Sketchpad", "Draw Filled Rectangle", 0));
        actionDraw_Filled_Polygon->setText(QApplication::translate("Sketchpad", "Draw Filled Polygon", 0));
        actionShutdown->setText(QApplication::translate("Sketchpad", "Shutdown", 0));
        actionLoad->setText(QApplication::translate("Sketchpad", "load", 0));
        actionCreate->setText(QApplication::translate("Sketchpad", "create", 0));
        actionActionFill->setText(QApplication::translate("Sketchpad", "actionFill", 0));
#ifndef QT_NO_TOOLTIP
        actionActionFill->setToolTip(QApplication::translate("Sketchpad", "Fill Region", 0));
#endif // QT_NO_TOOLTIP
        actionLoad_Photo->setText(QApplication::translate("Sketchpad", "Load Photo", 0));
#ifndef QT_NO_TOOLTIP
        actionLoad_Photo->setToolTip(QApplication::translate("Sketchpad", "Load photo", 0));
#endif // QT_NO_TOOLTIP
        actionLaunch_webcam->setText(QApplication::translate("Sketchpad", "Launch Webcam", 0));
#ifndef QT_NO_TOOLTIP
        actionLaunch_webcam->setToolTip(QApplication::translate("Sketchpad", "launch webcam", 0));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("Sketchpad", "File", 0));
        menuRobot->setTitle(QApplication::translate("Sketchpad", "Robot", 0));
        menuWorkspace->setTitle(QApplication::translate("Sketchpad", "Canvas", 0));
        menuImage->setTitle(QApplication::translate("Sketchpad", "Image", 0));
        toolBar_2->setWindowTitle(QApplication::translate("Sketchpad", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class Sketchpad: public Ui_Sketchpad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKETCHPAD_H
