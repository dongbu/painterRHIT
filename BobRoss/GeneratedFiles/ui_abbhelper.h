/********************************************************************************
** Form generated from reading UI file 'abbhelper.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABBHELPER_H
#define UI_ABBHELPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ABBHelper
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_SampleTitle;
    QLabel *label_SlotTitle;
    QLabel *label_Port;
    QDialogButtonBox *buttonBox;
    QSpinBox *spinBox_comNum;
    QLabel *label;

    void setupUi(QWidget *ABBHelper)
    {
        if (ABBHelper->objectName().isEmpty())
            ABBHelper->setObjectName(QStringLiteral("ABBHelper"));
        ABBHelper->resize(489, 375);
        gridLayoutWidget = new QWidget(ABBHelper);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 60, 451, 272));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_SampleTitle = new QLabel(gridLayoutWidget);
        label_SampleTitle->setObjectName(QStringLiteral("label_SampleTitle"));

        gridLayout->addWidget(label_SampleTitle, 0, 1, 1, 1);

        label_SlotTitle = new QLabel(gridLayoutWidget);
        label_SlotTitle->setObjectName(QStringLiteral("label_SlotTitle"));

        gridLayout->addWidget(label_SlotTitle, 0, 0, 1, 1);

        label_Port = new QLabel(ABBHelper);
        label_Port->setObjectName(QStringLiteral("label_Port"));
        label_Port->setGeometry(QRect(60, 20, 51, 21));
        buttonBox = new QDialogButtonBox(ABBHelper);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(130, 330, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        spinBox_comNum = new QSpinBox(ABBHelper);
        spinBox_comNum->setObjectName(QStringLiteral("spinBox_comNum"));
        spinBox_comNum->setGeometry(QRect(90, 20, 61, 22));
        spinBox_comNum->setMinimum(1);
        spinBox_comNum->setMaximum(50);
        label = new QLabel(ABBHelper);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(170, 20, 301, 16));

        retranslateUi(ABBHelper);

        QMetaObject::connectSlotsByName(ABBHelper);
    } // setupUi

    void retranslateUi(QWidget *ABBHelper)
    {
        ABBHelper->setWindowTitle(QApplication::translate("ABBHelper", "ABB Parameters", 0));
        label_SampleTitle->setText(QApplication::translate("ABBHelper", "Color", 0));
        label_SlotTitle->setText(QApplication::translate("ABBHelper", "Slot", 0));
        label_Port->setText(QApplication::translate("ABBHelper", "<html><head/><body><p><span style=\" font-size:10pt;\">Port:</span></p></body></html>", 0));
        spinBox_comNum->setPrefix(QApplication::translate("ABBHelper", "COM", 0));
        label->setText(QApplication::translate("ABBHelper", "<html><head/><body><p><span style=\" font-weight:600; color:#ff0000;\">Warning: once you hit ok, you cannot edit colors</span></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class ABBHelper: public Ui_ABBHelper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABBHELPER_H
