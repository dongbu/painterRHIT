/********************************************************************************
** Form generated from reading UI file 'commandviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDVIEWER_H
#define UI_COMMANDVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommandViewer
{
public:
    QAction *RunFromStart;
    QAction *Stop;
    QAction *Pause;
    QAction *StepForwards;
    QAction *StepBackwards;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *CommandListLayout;
    QLabel *Command_List_Label;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *AddCommand;
    QPushButton *EditCommand;
    QPushButton *DeleteCommand;
    QPushButton *MoveUp;
    QPushButton *MoveDown;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *CommandViewer)
    {
        if (CommandViewer->objectName().isEmpty())
            CommandViewer->setObjectName(QStringLiteral("CommandViewer"));
        CommandViewer->resize(605, 374);
        RunFromStart = new QAction(CommandViewer);
        RunFromStart->setObjectName(QStringLiteral("RunFromStart"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon Storage/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        RunFromStart->setIcon(icon);
        Stop = new QAction(CommandViewer);
        Stop->setObjectName(QStringLiteral("Stop"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon Storage/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        Stop->setIcon(icon1);
        Pause = new QAction(CommandViewer);
        Pause->setObjectName(QStringLiteral("Pause"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Icon Storage/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        Pause->setIcon(icon2);
        StepForwards = new QAction(CommandViewer);
        StepForwards->setObjectName(QStringLiteral("StepForwards"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Icon Storage/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        StepForwards->setIcon(icon3);
        StepBackwards = new QAction(CommandViewer);
        StepBackwards->setObjectName(QStringLiteral("StepBackwards"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Icon Storage/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        StepBackwards->setIcon(icon4);
        centralwidget = new QWidget(CommandViewer);
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
        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        horizontalLayout_2->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        AddCommand = new QPushButton(layoutWidget);
        AddCommand->setObjectName(QStringLiteral("AddCommand"));

        verticalLayout->addWidget(AddCommand);

        EditCommand = new QPushButton(layoutWidget);
        EditCommand->setObjectName(QStringLiteral("EditCommand"));

        verticalLayout->addWidget(EditCommand);

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

        CommandViewer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CommandViewer);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 605, 26));
        CommandViewer->setMenuBar(menubar);
        statusbar = new QStatusBar(CommandViewer);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        CommandViewer->setStatusBar(statusbar);
        toolBar = new QToolBar(CommandViewer);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        CommandViewer->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(RunFromStart);
        toolBar->addAction(Stop);
        toolBar->addAction(StepForwards);
        toolBar->addAction(Pause);
        toolBar->addAction(StepBackwards);

        retranslateUi(CommandViewer);

        QMetaObject::connectSlotsByName(CommandViewer);
    } // setupUi

    void retranslateUi(QMainWindow *CommandViewer)
    {
        CommandViewer->setWindowTitle(QApplication::translate("CommandViewer", "MainWindow", 0));
        RunFromStart->setText(QApplication::translate("CommandViewer", "Run", 0));
#ifndef QT_NO_TOOLTIP
        RunFromStart->setToolTip(QApplication::translate("CommandViewer", "start/continue drawing", 0));
#endif // QT_NO_TOOLTIP
        RunFromStart->setShortcut(QApplication::translate("CommandViewer", "Ctrl+R", 0));
        Stop->setText(QApplication::translate("CommandViewer", "Stop", 0));
#ifndef QT_NO_TOOLTIP
        Stop->setToolTip(QApplication::translate("CommandViewer", "stop drawing", 0));
#endif // QT_NO_TOOLTIP
        Pause->setText(QApplication::translate("CommandViewer", "Pause", 0));
#ifndef QT_NO_TOOLTIP
        Pause->setToolTip(QApplication::translate("CommandViewer", "Pause drawing", 0));
#endif // QT_NO_TOOLTIP
        StepForwards->setText(QApplication::translate("CommandViewer", "Next", 0));
#ifndef QT_NO_TOOLTIP
        StepForwards->setToolTip(QApplication::translate("CommandViewer", "draw next action", 0));
#endif // QT_NO_TOOLTIP
        StepBackwards->setText(QApplication::translate("CommandViewer", "Previous", 0));
#ifndef QT_NO_TOOLTIP
        StepBackwards->setToolTip(QApplication::translate("CommandViewer", "undo last draw", 0));
#endif // QT_NO_TOOLTIP
        Command_List_Label->setText(QApplication::translate("CommandViewer", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Command List</span></p><p align=\"center\"><br/></p></body></html>", 0));
        AddCommand->setText(QApplication::translate("CommandViewer", "Add Command", 0));
        EditCommand->setText(QApplication::translate("CommandViewer", "Edit", 0));
        DeleteCommand->setText(QApplication::translate("CommandViewer", "Delete", 0));
        MoveUp->setText(QApplication::translate("CommandViewer", "Move Up", 0));
        MoveDown->setText(QApplication::translate("CommandViewer", "Move Down", 0));
        toolBar->setWindowTitle(QApplication::translate("CommandViewer", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class CommandViewer: public Ui_CommandViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDVIEWER_H
