/********************************************************************************
** Form generated from reading UI file 'CommandWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDWINDOW_H
#define UI_COMMANDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommandWindow
{
public:
    QAction *actionPlay;
    QAction *actionClear;
    QAction *actionForward;
    QAction *actionBackward;
    QAction *actionPause;
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *MoveUp;
    QPushButton *MoveDown;
    QPushButton *DeleteCommand;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *CommandsRun;
    QSpacerItem *horizontalSpacer;
    QLabel *TimeEllapsed;
    QMenuBar *menubar;
    QMenu *menuSimulator;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *CommandWindow)
    {
        if (CommandWindow->objectName().isEmpty())
            CommandWindow->setObjectName(QStringLiteral("CommandWindow"));
        CommandWindow->resize(392, 398);
        actionPlay = new QAction(CommandWindow);
        actionPlay->setObjectName(QStringLiteral("actionPlay"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon Storage/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlay->setIcon(icon);
        actionClear = new QAction(CommandWindow);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon Storage/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon1);
        actionForward = new QAction(CommandWindow);
        actionForward->setObjectName(QStringLiteral("actionForward"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Icon Storage/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionForward->setIcon(icon2);
        actionBackward = new QAction(CommandWindow);
        actionBackward->setObjectName(QStringLiteral("actionBackward"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Icon Storage/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBackward->setIcon(icon3);
        actionPause = new QAction(CommandWindow);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Icon Storage/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon4);
        centralwidget = new QWidget(CommandWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 371, 261));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(horizontalLayoutWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout->addWidget(tableWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        MoveUp = new QPushButton(horizontalLayoutWidget);
        MoveUp->setObjectName(QStringLiteral("MoveUp"));

        verticalLayout->addWidget(MoveUp);

        MoveDown = new QPushButton(horizontalLayoutWidget);
        MoveDown->setObjectName(QStringLiteral("MoveDown"));

        verticalLayout->addWidget(MoveDown);

        DeleteCommand = new QPushButton(horizontalLayoutWidget);
        DeleteCommand->setObjectName(QStringLiteral("DeleteCommand"));

        verticalLayout->addWidget(DeleteCommand);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayoutWidget_2 = new QWidget(centralwidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(19, 270, 241, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        CommandsRun = new QLabel(horizontalLayoutWidget_2);
        CommandsRun->setObjectName(QStringLiteral("CommandsRun"));

        horizontalLayout_2->addWidget(CommandsRun);

        horizontalSpacer = new QSpacerItem(118, 26, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        TimeEllapsed = new QLabel(horizontalLayoutWidget_2);
        TimeEllapsed->setObjectName(QStringLiteral("TimeEllapsed"));

        horizontalLayout_2->addWidget(TimeEllapsed);

        CommandWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CommandWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 392, 26));
        menuSimulator = new QMenu(menubar);
        menuSimulator->setObjectName(QStringLiteral("menuSimulator"));
        CommandWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CommandWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        CommandWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(CommandWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        CommandWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuSimulator->menuAction());
        menuSimulator->addAction(actionPlay);
        menuSimulator->addAction(actionClear);
        menuSimulator->addAction(actionPause);
        menuSimulator->addAction(actionForward);
        menuSimulator->addAction(actionBackward);
        toolBar->addAction(actionPlay);
        toolBar->addAction(actionClear);
        toolBar->addAction(actionBackward);
        toolBar->addAction(actionPause);
        toolBar->addAction(actionForward);

        retranslateUi(CommandWindow);

        QMetaObject::connectSlotsByName(CommandWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CommandWindow)
    {
        CommandWindow->setWindowTitle(QApplication::translate("CommandWindow", "Command List", 0));
        actionPlay->setText(QApplication::translate("CommandWindow", "Play", 0));
#ifndef QT_NO_TOOLTIP
        actionPlay->setToolTip(QApplication::translate("CommandWindow", "Runs the simulation", 0));
#endif // QT_NO_TOOLTIP
        actionClear->setText(QApplication::translate("CommandWindow", "Clear", 0));
#ifndef QT_NO_TOOLTIP
        actionClear->setToolTip(QApplication::translate("CommandWindow", "clear simulation", 0));
#endif // QT_NO_TOOLTIP
        actionForward->setText(QApplication::translate("CommandWindow", "Forward", 0));
#ifndef QT_NO_TOOLTIP
        actionForward->setToolTip(QApplication::translate("CommandWindow", "steps forward", 0));
#endif // QT_NO_TOOLTIP
        actionBackward->setText(QApplication::translate("CommandWindow", "Backward", 0));
#ifndef QT_NO_TOOLTIP
        actionBackward->setToolTip(QApplication::translate("CommandWindow", "step backward", 0));
#endif // QT_NO_TOOLTIP
        actionPause->setText(QApplication::translate("CommandWindow", "Pause", 0));
#ifndef QT_NO_TOOLTIP
        actionPause->setToolTip(QApplication::translate("CommandWindow", "pause simulation", 0));
#endif // QT_NO_TOOLTIP
        MoveUp->setText(QApplication::translate("CommandWindow", "Move Up", 0));
        MoveDown->setText(QApplication::translate("CommandWindow", "Move Down", 0));
        DeleteCommand->setText(QApplication::translate("CommandWindow", "Delete", 0));
        CommandsRun->setText(QApplication::translate("CommandWindow", "x/x", 0));
        TimeEllapsed->setText(QApplication::translate("CommandWindow", "0:00 ellapsed", 0));
        menuSimulator->setTitle(QApplication::translate("CommandWindow", "Simulator", 0));
        toolBar->setWindowTitle(QApplication::translate("CommandWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class CommandWindow: public Ui_CommandWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDWINDOW_H
