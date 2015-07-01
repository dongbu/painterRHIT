/********************************************************************************
** Form generated from reading UI file 'commandeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDEDITOR_H
#define UI_COMMANDEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommandEditor
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *CommandEditor)
    {
        if (CommandEditor->objectName().isEmpty())
            CommandEditor->setObjectName(QStringLiteral("CommandEditor"));
        CommandEditor->resize(447, 349);
        verticalLayoutWidget = new QWidget(CommandEditor);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 421, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(CommandEditor);

        QMetaObject::connectSlotsByName(CommandEditor);
    } // setupUi

    void retranslateUi(QWidget *CommandEditor)
    {
        CommandEditor->setWindowTitle(QApplication::translate("CommandEditor", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CommandEditor: public Ui_CommandEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDEDITOR_H
