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
    QAction *actionStop;
    QAction *actionForward;
    QAction *actionBackward;
    QAction *actionPause;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *CommandListLayout;
    QLabel *Command_List_Label;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *DeleteCommand;
    QPushButton *MoveUp;
    QPushButton *MoveDown;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menubar;
    QMenu *menuSimulator;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *CommandWindow)
    {
        if (CommandWindow->objectName().isEmpty())
            CommandWindow->setObjectName(QStringLiteral("CommandWindow"));
        CommandWindow->resize(593, 362);
        actionPlay = new QAction(CommandWindow);
        actionPlay->setObjectName(QStringLiteral("actionPlay"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon Storage/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlay->setIcon(icon);
        actionStop = new QAction(CommandWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon Storage/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon1);
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
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 571, 258));
        CommandListLayout = new QVBoxLayout(layoutWidget);
        CommandListLayout->setObjectName(QStringLiteral("CommandListLayout"));
        CommandListLayout->setContentsMargins(0, 0, 0, 0);
        Command_List_Label = new QLabel(layoutWidget);
        Command_List_Label->setObjectName(QStringLiteral("Command_List_Label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Command_List_Label->sizePolicy().hasHeightForWidth());
        Command_List_Label->setSizePolicy(sizePolicy);

        CommandListLayout->addWidget(Command_List_Label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tableWidget = new QTableWidget(layoutWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout_2->addWidget(tableWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        DeleteCommand = new QPushButton(layoutWidget);
        DeleteCommand->setObjectName(QStringLiteral("DeleteCommand"));

        verticalLayout->addWidget(DeleteCommand);

        MoveUp = new QPushButton(layoutWidget);
        MoveUp->setObjectName(QStringLiteral("MoveUp"));

        verticalLayout->addWidget(MoveUp);

        MoveDown = new QPushButton(layoutWidget);
        MoveDown->setObjectName(QStringLiteral("MoveDown"));

        verticalLayout->addWidget(MoveDown);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));

        horizontalLayout_2->addLayout(verticalLayout_2);


        CommandListLayout->addLayout(horizontalLayout_2);

        CommandWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CommandWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 593, 26));
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
        menuSimulator->addAction(actionStop);
        menuSimulator->addAction(actionPause);
        menuSimulator->addAction(actionForward);
        menuSimulator->addAction(actionBackward);
        toolBar->addAction(actionPlay);
        toolBar->addAction(actionStop);
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
        actionStop->setText(QApplication::translate("CommandWindow", "Stop", 0));
#ifndef QT_NO_TOOLTIP
        actionStop->setToolTip(QApplication::translate("CommandWindow", "stops the simulation", 0));
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
        Command_List_Label->setText(QApplication::translate("CommandWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Command List</span></p><p align=\"center\"><br/></p></body></html>", 0));
        DeleteCommand->setText(QApplication::translate("CommandWindow", "Delete", 0));
        MoveUp->setText(QApplication::translate("CommandWindow", "Move Up", 0));
        MoveDown->setText(QApplication::translate("CommandWindow", "Move Down", 0));
        menuSimulator->setTitle(QApplication::translate("CommandWindow", "Simulator", 0));
        toolBar->setWindowTitle(QApplication::translate("CommandWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class CommandWindow: public Ui_CommandWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDWINDOW_H
