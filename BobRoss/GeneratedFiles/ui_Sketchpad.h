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
    QAction *actionDraw_Function;
    QAction *actionDraw_Point_Map;
    QAction *actionConnect;
    QAction *actionSave_As;
    QAction *actionSave;
    QWidget *centralwidget;
    QWidget *widget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *Sketchpad)
    {
        if (Sketchpad->objectName().isEmpty())
            Sketchpad->setObjectName(QStringLiteral("Sketchpad"));
        Sketchpad->resize(960, 852);
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
        actionDraw_Function = new QAction(Sketchpad);
        actionDraw_Function->setObjectName(QStringLiteral("actionDraw_Function"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Icon Storage/function.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Function->setIcon(icon5);
        actionDraw_Point_Map = new QAction(Sketchpad);
        actionDraw_Point_Map->setObjectName(QStringLiteral("actionDraw_Point_Map"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Icon Storage/Point Map.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Point_Map->setIcon(icon6);
        actionConnect = new QAction(Sketchpad);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        actionSave_As = new QAction(Sketchpad);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Icon Storage/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon7);
        actionSave = new QAction(Sketchpad);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave->setIcon(icon7);
        centralwidget = new QWidget(Sketchpad);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 901, 750));
        widget->setCursor(QCursor(Qt::CrossCursor));
        Sketchpad->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Sketchpad);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 960, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        Sketchpad->setMenuBar(menubar);
        statusbar = new QStatusBar(Sketchpad);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Sketchpad->setStatusBar(statusbar);
        toolBar = new QToolBar(Sketchpad);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        Sketchpad->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(Sketchpad);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        Sketchpad->addToolBar(Qt::LeftToolBarArea, toolBar_2);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionSave);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionOpen);
        toolBar_2->addAction(actionDraw_Square);
        toolBar_2->addAction(actionDraw_Circle);
        toolBar_2->addAction(actionDraw_Line);
        toolBar_2->addAction(actionDraw_Point_Map);

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
        actionDraw_Function->setText(QApplication::translate("Sketchpad", "Draw Function", 0));
        actionDraw_Point_Map->setText(QApplication::translate("Sketchpad", "Draw Point Map", 0));
        actionConnect->setText(QApplication::translate("Sketchpad", "Connect", 0));
        actionSave_As->setText(QApplication::translate("Sketchpad", "Save As", 0));
        actionSave->setText(QApplication::translate("Sketchpad", "Save", 0));
        actionSave->setShortcut(QApplication::translate("Sketchpad", "Ctrl+S", 0));
        menuFile->setTitle(QApplication::translate("Sketchpad", "file", 0));
        toolBar->setWindowTitle(QApplication::translate("Sketchpad", "toolBar", 0));
        toolBar_2->setWindowTitle(QApplication::translate("Sketchpad", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class Sketchpad: public Ui_Sketchpad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKETCHPAD_H
