/********************************************************************************
** Form generated from reading UI file 'colorChangeUI.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORCHANGEUI_H
#define UI_COLORCHANGEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_colorChange
{
public:
    QTableWidget *tableWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *ChangeAllLabel;
    QCheckBox *ChangeAll;
    QSpacerItem *horizontalSpacer;
    QPushButton *ConfirmButton;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *ChangeAllLabel_2;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *colorChange)
    {
        if (colorChange->objectName().isEmpty())
            colorChange->setObjectName(QStringLiteral("colorChange"));
        colorChange->resize(460, 163);
        tableWidget = new QTableWidget(colorChange);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem3);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 40, 441, 61));
        layoutWidget = new QWidget(colorChange);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 120, 441, 30));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ChangeAllLabel = new QLabel(layoutWidget);
        ChangeAllLabel->setObjectName(QStringLiteral("ChangeAllLabel"));

        horizontalLayout->addWidget(ChangeAllLabel);

        ChangeAll = new QCheckBox(layoutWidget);
        ChangeAll->setObjectName(QStringLiteral("ChangeAll"));

        horizontalLayout->addWidget(ChangeAll);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ConfirmButton = new QPushButton(layoutWidget);
        ConfirmButton->setObjectName(QStringLiteral("ConfirmButton"));

        horizontalLayout->addWidget(ConfirmButton);

        layoutWidget1 = new QWidget(colorChange);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 10, 441, 24));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        ChangeAllLabel_2 = new QLabel(layoutWidget1);
        ChangeAllLabel_2->setObjectName(QStringLiteral("ChangeAllLabel_2"));

        horizontalLayout_2->addWidget(ChangeAllLabel_2);

        comboBox = new QComboBox(layoutWidget1);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_2->addWidget(comboBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        retranslateUi(colorChange);

        QMetaObject::connectSlotsByName(colorChange);
    } // setupUi

    void retranslateUi(QDialog *colorChange)
    {
        colorChange->setWindowTitle(QApplication::translate("colorChange", "Dialog", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("colorChange", "rgb", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("colorChange", "break", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("colorChange", "command type", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("colorChange", "1", 0));
        ChangeAllLabel->setText(QApplication::translate("colorChange", "Change all instances of this color", 0));
        ChangeAll->setText(QString());
        ConfirmButton->setText(QApplication::translate("colorChange", "Confirm", 0));
        ChangeAllLabel_2->setText(QApplication::translate("colorChange", "Pick a new color:  ", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("colorChange", "current color", 0)
         << QApplication::translate("colorChange", "black", 0)
         << QApplication::translate("colorChange", "orange", 0)
         << QApplication::translate("colorChange", "yellow", 0)
         << QApplication::translate("colorChange", "green", 0)
         << QApplication::translate("colorChange", "red", 0)
         << QApplication::translate("colorChange", "blue", 0)
         << QApplication::translate("colorChange", "purple", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class colorChange: public Ui_colorChange {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORCHANGEUI_H
