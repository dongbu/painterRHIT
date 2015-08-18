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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
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
    QWidget *FormWidget;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QRadioButton *radioButton;
    QLabel *label;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QLabel *label_3;
    QLabel *label_2;
    QPushButton *RFButton;

    void setupUi(QWidget *WorkspaceWizard)
    {
        if (WorkspaceWizard->objectName().isEmpty())
            WorkspaceWizard->setObjectName(QStringLiteral("WorkspaceWizard"));
        WorkspaceWizard->resize(496, 467);
        Title = new QLabel(WorkspaceWizard);
        Title->setObjectName(QStringLiteral("Title"));
        Title->setGeometry(QRect(10, 10, 441, 21));
        ForwardButton = new QPushButton(WorkspaceWizard);
        ForwardButton->setObjectName(QStringLiteral("ForwardButton"));
        ForwardButton->setGeometry(QRect(410, 430, 75, 23));
        BackButton = new QPushButton(WorkspaceWizard);
        BackButton->setObjectName(QStringLiteral("BackButton"));
        BackButton->setGeometry(QRect(320, 430, 75, 23));
        Directions = new QLabel(WorkspaceWizard);
        Directions->setObjectName(QStringLiteral("Directions"));
        Directions->setGeometry(QRect(10, 50, 471, 111));
        QuestionButton = new QPushButton(WorkspaceWizard);
        QuestionButton->setObjectName(QStringLiteral("QuestionButton"));
        QuestionButton->setGeometry(QRect(460, 10, 21, 21));
        verticalLayoutWidget = new QWidget(WorkspaceWizard);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 170, 471, 241));
        ItemLayout = new QVBoxLayout(verticalLayoutWidget);
        ItemLayout->setObjectName(QStringLiteral("ItemLayout"));
        ItemLayout->setContentsMargins(0, 0, 0, 0);
        FormWidget = new QWidget(verticalLayoutWidget);
        FormWidget->setObjectName(QStringLiteral("FormWidget"));
        layoutWidget = new QWidget(FormWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 551, 91));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(layoutWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        formLayout->setWidget(0, QFormLayout::LabelRole, radioButton);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label);

        radioButton_2 = new QRadioButton(layoutWidget);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, radioButton_2);

        radioButton_3 = new QRadioButton(layoutWidget);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, radioButton_3);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, label_3);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, label_2);


        ItemLayout->addWidget(FormWidget);

        RFButton = new QPushButton(WorkspaceWizard);
        RFButton->setObjectName(QStringLiteral("RFButton"));
        RFButton->setGeometry(QRect(10, 420, 131, 23));

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
        radioButton->setText(QApplication::translate("WorkspaceWizard", "Top-Left", 0));
        label->setText(QApplication::translate("WorkspaceWizard", "undefined", 0));
        radioButton_2->setText(QApplication::translate("WorkspaceWizard", "Top-Right", 0));
        radioButton_3->setText(QApplication::translate("WorkspaceWizard", "Bottom-Left", 0));
        label_3->setText(QApplication::translate("WorkspaceWizard", "undefined", 0));
        label_2->setText(QApplication::translate("WorkspaceWizard", "undefined", 0));
        RFButton->setText(QApplication::translate("WorkspaceWizard", "Regain Robot Focus", 0));
    } // retranslateUi

};

namespace Ui {
    class WorkspaceWizard: public Ui_WorkspaceWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKSPACEWIZARD_H
