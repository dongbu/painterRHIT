/********************************************************************************
** Form generated from reading UI file 'solid.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLID_H
#define UI_SOLID_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Solid
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *Solid)
    {
        if (Solid->objectName().isEmpty())
            Solid->setObjectName(QStringLiteral("Solid"));
        Solid->resize(441, 342);
        verticalLayoutWidget = new QWidget(Solid);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 421, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(Solid);

        QMetaObject::connectSlotsByName(Solid);
    } // setupUi

    void retranslateUi(QWidget *Solid)
    {
        Solid->setWindowTitle(QApplication::translate("Solid", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class Solid: public Ui_Solid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLID_H
