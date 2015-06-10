/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionNew;
    QAction *actionDraw_Square;
    QAction *actionDraw_Circle;
    QAction *actionDraw_Line;
    QAction *actionDraw_Function;
    QAction *actionDraw_Point_Map;
    QAction *actionRun;
    QAction *actionStop;
    QAction *actionPause;
    QAction *actionNext;
    QAction *actionPrevious;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QLabel *Command_List_Label;
    QToolButton *toolButton;
    QFrame *line;
    QFrame *line_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *CommandEditLayout;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuNew_Window;
    QMenu *menuDraw_Commands;
    QMenu *menuDebugger;
    QToolBar *GeneralCommands;
    QToolBar *DrawFunctions;
    QToolBar *DebugFunctions;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1921, 989);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon Storage/open.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon Storage/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionSave_As->setIcon(icon1);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Icon Storage/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon2);
        actionDraw_Square = new QAction(MainWindow);
        actionDraw_Square->setObjectName(QStringLiteral("actionDraw_Square"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Icon Storage/Square.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Square->setIcon(icon3);
        actionDraw_Circle = new QAction(MainWindow);
        actionDraw_Circle->setObjectName(QStringLiteral("actionDraw_Circle"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Icon Storage/Circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Circle->setIcon(icon4);
        actionDraw_Line = new QAction(MainWindow);
        actionDraw_Line->setObjectName(QStringLiteral("actionDraw_Line"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Icon Storage/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Line->setIcon(icon5);
        actionDraw_Function = new QAction(MainWindow);
        actionDraw_Function->setObjectName(QStringLiteral("actionDraw_Function"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Icon Storage/function.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Function->setIcon(icon6);
        actionDraw_Point_Map = new QAction(MainWindow);
        actionDraw_Point_Map->setObjectName(QStringLiteral("actionDraw_Point_Map"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Icon Storage/Point Map.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Point_Map->setIcon(icon7);
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/Icon Storage/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon8);
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/Icon Storage/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon9);
        actionPause = new QAction(MainWindow);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/Icon Storage/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon10);
        actionNext = new QAction(MainWindow);
        actionNext->setObjectName(QStringLiteral("actionNext"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/Icon Storage/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon11);
        actionPrevious = new QAction(MainWindow);
        actionPrevious->setObjectName(QStringLiteral("actionPrevious"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/Icon Storage/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon12);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(1, 715, 1099, 170));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout);

        Command_List_Label = new QLabel(centralWidget);
        Command_List_Label->setObjectName(QStringLiteral("Command_List_Label"));
        Command_List_Label->setGeometry(QRect(0, 670, 1101, 31));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Command_List_Label->sizePolicy().hasHeightForWidth());
        Command_List_Label->setSizePolicy(sizePolicy);
        toolButton = new QToolButton(centralWidget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(1080, 0, 25, 19));
        toolButton->setCheckable(false);
        toolButton->setChecked(false);
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(1110, 0, 20, 931));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(-3, 660, 1121, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(670, 10, 391, 411));
        CommandEditLayout = new QVBoxLayout(verticalLayoutWidget);
        CommandEditLayout->setSpacing(6);
        CommandEditLayout->setContentsMargins(11, 11, 11, 11);
        CommandEditLayout->setObjectName(QStringLiteral("CommandEditLayout"));
        CommandEditLayout->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1921, 21));
        menuNew_Window = new QMenu(menuBar);
        menuNew_Window->setObjectName(QStringLiteral("menuNew_Window"));
        menuDraw_Commands = new QMenu(menuBar);
        menuDraw_Commands->setObjectName(QStringLiteral("menuDraw_Commands"));
        menuDebugger = new QMenu(menuBar);
        menuDebugger->setObjectName(QStringLiteral("menuDebugger"));
        MainWindow->setMenuBar(menuBar);
        GeneralCommands = new QToolBar(MainWindow);
        GeneralCommands->setObjectName(QStringLiteral("GeneralCommands"));
        MainWindow->addToolBar(Qt::TopToolBarArea, GeneralCommands);
        DrawFunctions = new QToolBar(MainWindow);
        DrawFunctions->setObjectName(QStringLiteral("DrawFunctions"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, DrawFunctions);
        DebugFunctions = new QToolBar(MainWindow);
        DebugFunctions->setObjectName(QStringLiteral("DebugFunctions"));
        MainWindow->addToolBar(Qt::TopToolBarArea, DebugFunctions);

        menuBar->addAction(menuNew_Window->menuAction());
        menuBar->addAction(menuDraw_Commands->menuAction());
        menuBar->addAction(menuDebugger->menuAction());
        menuNew_Window->addAction(actionOpen);
        menuNew_Window->addAction(actionSave);
        menuNew_Window->addAction(actionSave_As);
        menuNew_Window->addAction(actionNew);
        menuDraw_Commands->addAction(actionDraw_Square);
        menuDraw_Commands->addAction(actionDraw_Circle);
        menuDraw_Commands->addAction(actionDraw_Line);
        menuDraw_Commands->addAction(actionDraw_Function);
        menuDraw_Commands->addAction(actionDraw_Point_Map);
        menuDebugger->addAction(actionStop);
        menuDebugger->addAction(actionPause);
        menuDebugger->addAction(actionNext);
        menuDebugger->addAction(actionPrevious);
        GeneralCommands->addAction(actionOpen);
        GeneralCommands->addAction(actionNew);
        GeneralCommands->addAction(actionSave);
        GeneralCommands->addSeparator();
        DrawFunctions->addAction(actionDraw_Square);
        DrawFunctions->addAction(actionDraw_Circle);
        DrawFunctions->addAction(actionDraw_Line);
        DrawFunctions->addAction(actionDraw_Point_Map);
        DrawFunctions->addAction(actionDraw_Function);
        DebugFunctions->addAction(actionRun);
        DebugFunctions->addAction(actionStop);
        DebugFunctions->addAction(actionPrevious);
        DebugFunctions->addAction(actionPause);
        DebugFunctions->addAction(actionNext);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        actionDraw_Square->setText(QApplication::translate("MainWindow", "Draw Square", 0));
        actionDraw_Circle->setText(QApplication::translate("MainWindow", "Draw Circle", 0));
        actionDraw_Line->setText(QApplication::translate("MainWindow", "Draw Line", 0));
        actionDraw_Function->setText(QApplication::translate("MainWindow", "Draw Function", 0));
        actionDraw_Point_Map->setText(QApplication::translate("MainWindow", "Draw Point Map", 0));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0));
#ifndef QT_NO_TOOLTIP
        actionRun->setToolTip(QApplication::translate("MainWindow", "start/continue drawing", 0));
#endif // QT_NO_TOOLTIP
        actionRun->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", 0));
#ifndef QT_NO_TOOLTIP
        actionStop->setToolTip(QApplication::translate("MainWindow", "stop drawing", 0));
#endif // QT_NO_TOOLTIP
        actionPause->setText(QApplication::translate("MainWindow", "Pause", 0));
#ifndef QT_NO_TOOLTIP
        actionPause->setToolTip(QApplication::translate("MainWindow", "Pause drawing", 0));
#endif // QT_NO_TOOLTIP
        actionNext->setText(QApplication::translate("MainWindow", "Next", 0));
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("MainWindow", "draw next action", 0));
#endif // QT_NO_TOOLTIP
        actionPrevious->setText(QApplication::translate("MainWindow", "Previous", 0));
#ifndef QT_NO_TOOLTIP
        actionPrevious->setToolTip(QApplication::translate("MainWindow", "undo last draw", 0));
#endif // QT_NO_TOOLTIP
        pushButton->setText(QApplication::translate("MainWindow", "Move Up", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Move Down", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Delete", 0));
        Command_List_Label->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Command List</span></p><p align=\"center\"><br/></p></body></html>", 0));
        toolButton->setText(QApplication::translate("MainWindow", "...", 0));
        menuNew_Window->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuDraw_Commands->setTitle(QApplication::translate("MainWindow", "Draw Commands", 0));
        menuDebugger->setTitle(QApplication::translate("MainWindow", "Debugger", 0));
        GeneralCommands->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        DrawFunctions->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
        DebugFunctions->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
