/********************************************************************************
** Form generated from reading UI file 'workspacewizard.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKSPACEWIZARD_H
#define UI_WORKSPACEWIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorkspaceWizard
{
public:
    QLabel *Title;
    QPushButton *ForwardButton;
    QPushButton *BackButton;
    QLabel *Directions;
    QPushButton *QuestionButton;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *ItemLayout;

    void setupUi(QWidget *WorkspaceWizard)
    {
        if (WorkspaceWizard->objectName().isEmpty())
            WorkspaceWizard->setObjectName(QStringLiteral("WorkspaceWizard"));
        WorkspaceWizard->resize(400, 300);
        Title = new QLabel(WorkspaceWizard);
        Title->setObjectName(QStringLiteral("Title"));
        Title->setGeometry(QRect(10, 10, 381, 21));
        ForwardButton = new QPushButton(WorkspaceWizard);
        ForwardButton->setObjectName(QStringLiteral("ForwardButton"));
        ForwardButton->setGeometry(QRect(310, 260, 75, 23));
        BackButton = new QPushButton(WorkspaceWizard);
        BackButton->setObjectName(QStringLiteral("BackButton"));
        BackButton->setGeometry(QRect(220, 260, 75, 23));
        Directions = new QLabel(WorkspaceWizard);
        Directions->setObjectName(QStringLiteral("Directions"));
        Directions->setGeometry(QRect(10, 50, 361, 91));
        QuestionButton = new QPushButton(WorkspaceWizard);
        QuestionButton->setObjectName(QStringLiteral("QuestionButton"));
        QuestionButton->setGeometry(QRect(370, 10, 21, 21));
        verticalLayoutWidget = new QWidget(WorkspaceWizard);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 150, 371, 80));
        ItemLayout = new QVBoxLayout(verticalLayoutWidget);
        ItemLayout->setObjectName(QStringLiteral("ItemLayout"));
        ItemLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(WorkspaceWizard);

        QMetaObject::connectSlotsByName(WorkspaceWizard);
    } // setupUi

    void retranslateUi(QWidget *WorkspaceWizard)
    {
        WorkspaceWizard->setWindowTitle(QApplication::translate("WorkspaceWizard", "Form", 0));
        Title->setText(QApplication::translate("WorkspaceWizard", "Welcome to the Workspace Wizard", 0));
        ForwardButton->setText(QApplication::translate("WorkspaceWizard", "Next", 0));
        BackButton->setText(QApplication::translate("WorkspaceWizard", "Cancel", 0));
        Directions->setText(QApplication::translate("WorkspaceWizard", "Press \"Next\"' to continue", 0));
        QuestionButton->setText(QApplication::translate("WorkspaceWizard", "?", 0));
    } // retranslateUi

};

namespace Ui {
    class WorkspaceWizard: public Ui_WorkspaceWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKSPACEWIZARD_H
