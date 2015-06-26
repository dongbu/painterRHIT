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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommandViewer
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *CommandListLayout;
    QLabel *Command_List_Label;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *EditCommand;
    QPushButton *DeleteCommand;
    QPushButton *MoveUp;
    QPushButton *MoveDown;

    void setupUi(QWidget *CommandViewer)
    {
        if (CommandViewer->objectName().isEmpty())
            CommandViewer->setObjectName(QStringLiteral("CommandViewer"));
        CommandViewer->resize(592, 243);
        layoutWidget = new QWidget(CommandViewer);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 571, 223));
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

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        EditCommand = new QPushButton(layoutWidget);
        EditCommand->setObjectName(QStringLiteral("EditCommand"));

        verticalLayout_2->addWidget(EditCommand);

        DeleteCommand = new QPushButton(layoutWidget);
        DeleteCommand->setObjectName(QStringLiteral("DeleteCommand"));

        verticalLayout_2->addWidget(DeleteCommand);

        MoveUp = new QPushButton(layoutWidget);
        MoveUp->setObjectName(QStringLiteral("MoveUp"));

        verticalLayout_2->addWidget(MoveUp);

        MoveDown = new QPushButton(layoutWidget);
        MoveDown->setObjectName(QStringLiteral("MoveDown"));

        verticalLayout_2->addWidget(MoveDown);


        horizontalLayout_2->addLayout(verticalLayout_2);


        CommandListLayout->addLayout(horizontalLayout_2);


        retranslateUi(CommandViewer);

        QMetaObject::connectSlotsByName(CommandViewer);
    } // setupUi

    void retranslateUi(QWidget *CommandViewer)
    {
        CommandViewer->setWindowTitle(QApplication::translate("CommandViewer", "Form", 0));
        Command_List_Label->setText(QApplication::translate("CommandViewer", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Command List</span></p><p align=\"center\"><br/></p></body></html>", 0));
        EditCommand->setText(QApplication::translate("CommandViewer", "Edit", 0));
        DeleteCommand->setText(QApplication::translate("CommandViewer", "Delete", 0));
        MoveUp->setText(QApplication::translate("CommandViewer", "Move Up", 0));
        MoveDown->setText(QApplication::translate("CommandViewer", "Move Down", 0));
    } // retranslateUi

};

namespace Ui {
    class CommandViewer: public Ui_CommandViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDVIEWER_H
