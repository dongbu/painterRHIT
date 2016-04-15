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
#include <QtWidgets/QCheckBox>
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
    QCheckBox *checkBox_c7;
    QSpinBox *spinBox_c7R;
    QSpinBox *spinBox_c7G;
    QSpinBox *spinBox_c7B;
    QSpinBox *spinBox_c8R;
    QSpinBox *spinBox_c8G;
    QLabel *label_BlueTitle;
    QLabel *label_SampleTitle;
    QLabel *label_GreenTitle;
    QLabel *label_RedTitle;
    QCheckBox *checkBox_c2;
    QCheckBox *checkBox_c4;
    QCheckBox *checkBox_c6;
    QCheckBox *checkBox_c1;
    QCheckBox *checkBox_c5;
    QCheckBox *checkBox_c3;
    QLabel *label_SlotTitle;
    QLabel *label_c1Sample;
    QLabel *label_c2Sample;
    QLabel *label_c3Sample;
    QLabel *label_c4Sample;
    QLabel *label_c5Sample;
    QLabel *label_c6Sample;
    QSpinBox *spinBox_c1R;
    QSpinBox *spinBox_c1G;
    QSpinBox *spinBox_c1B;
    QSpinBox *spinBox_c2R;
    QSpinBox *spinBox_c2G;
    QSpinBox *spinBox_c2B;
    QSpinBox *spinBox_c3R;
    QSpinBox *spinBox_c3G;
    QSpinBox *spinBox_c3B;
    QSpinBox *spinBox_c4R;
    QSpinBox *spinBox_c4G;
    QSpinBox *spinBox_c4B;
    QSpinBox *spinBox_c5R;
    QSpinBox *spinBox_c5G;
    QSpinBox *spinBox_c5B;
    QSpinBox *spinBox_c6R;
    QSpinBox *spinBox_c6G;
    QSpinBox *spinBox_c6B;
    QCheckBox *checkBox_c8;
    QSpinBox *spinBox_c8B;
    QLabel *label_c7Sample;
    QLabel *label_c8Sample;
    QLabel *label_Port;
    QDialogButtonBox *buttonBox;
    QSpinBox *spinBox_comNum;

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
        checkBox_c7 = new QCheckBox(gridLayoutWidget);
        checkBox_c7->setObjectName(QStringLiteral("checkBox_c7"));

        gridLayout->addWidget(checkBox_c7, 7, 0, 1, 1);

        spinBox_c7R = new QSpinBox(gridLayoutWidget);
        spinBox_c7R->setObjectName(QStringLiteral("spinBox_c7R"));
        spinBox_c7R->setEnabled(false);
        spinBox_c7R->setMaximum(255);

        gridLayout->addWidget(spinBox_c7R, 7, 1, 1, 1);

        spinBox_c7G = new QSpinBox(gridLayoutWidget);
        spinBox_c7G->setObjectName(QStringLiteral("spinBox_c7G"));
        spinBox_c7G->setEnabled(false);
        spinBox_c7G->setMaximum(255);

        gridLayout->addWidget(spinBox_c7G, 7, 2, 1, 1);

        spinBox_c7B = new QSpinBox(gridLayoutWidget);
        spinBox_c7B->setObjectName(QStringLiteral("spinBox_c7B"));
        spinBox_c7B->setEnabled(false);
        spinBox_c7B->setMaximum(255);

        gridLayout->addWidget(spinBox_c7B, 7, 3, 1, 1);

        spinBox_c8R = new QSpinBox(gridLayoutWidget);
        spinBox_c8R->setObjectName(QStringLiteral("spinBox_c8R"));
        spinBox_c8R->setEnabled(false);
        spinBox_c8R->setMaximum(255);

        gridLayout->addWidget(spinBox_c8R, 8, 1, 1, 1);

        spinBox_c8G = new QSpinBox(gridLayoutWidget);
        spinBox_c8G->setObjectName(QStringLiteral("spinBox_c8G"));
        spinBox_c8G->setEnabled(false);
        spinBox_c8G->setMaximum(255);

        gridLayout->addWidget(spinBox_c8G, 8, 2, 1, 1);

        label_BlueTitle = new QLabel(gridLayoutWidget);
        label_BlueTitle->setObjectName(QStringLiteral("label_BlueTitle"));

        gridLayout->addWidget(label_BlueTitle, 0, 3, 1, 1);

        label_SampleTitle = new QLabel(gridLayoutWidget);
        label_SampleTitle->setObjectName(QStringLiteral("label_SampleTitle"));

        gridLayout->addWidget(label_SampleTitle, 0, 4, 1, 1);

        label_GreenTitle = new QLabel(gridLayoutWidget);
        label_GreenTitle->setObjectName(QStringLiteral("label_GreenTitle"));

        gridLayout->addWidget(label_GreenTitle, 0, 2, 1, 1);

        label_RedTitle = new QLabel(gridLayoutWidget);
        label_RedTitle->setObjectName(QStringLiteral("label_RedTitle"));

        gridLayout->addWidget(label_RedTitle, 0, 1, 1, 1);

        checkBox_c2 = new QCheckBox(gridLayoutWidget);
        checkBox_c2->setObjectName(QStringLiteral("checkBox_c2"));

        gridLayout->addWidget(checkBox_c2, 2, 0, 1, 1);

        checkBox_c4 = new QCheckBox(gridLayoutWidget);
        checkBox_c4->setObjectName(QStringLiteral("checkBox_c4"));

        gridLayout->addWidget(checkBox_c4, 4, 0, 1, 1);

        checkBox_c6 = new QCheckBox(gridLayoutWidget);
        checkBox_c6->setObjectName(QStringLiteral("checkBox_c6"));

        gridLayout->addWidget(checkBox_c6, 6, 0, 1, 1);

        checkBox_c1 = new QCheckBox(gridLayoutWidget);
        checkBox_c1->setObjectName(QStringLiteral("checkBox_c1"));

        gridLayout->addWidget(checkBox_c1, 1, 0, 1, 1);

        checkBox_c5 = new QCheckBox(gridLayoutWidget);
        checkBox_c5->setObjectName(QStringLiteral("checkBox_c5"));

        gridLayout->addWidget(checkBox_c5, 5, 0, 1, 1);

        checkBox_c3 = new QCheckBox(gridLayoutWidget);
        checkBox_c3->setObjectName(QStringLiteral("checkBox_c3"));

        gridLayout->addWidget(checkBox_c3, 3, 0, 1, 1);

        label_SlotTitle = new QLabel(gridLayoutWidget);
        label_SlotTitle->setObjectName(QStringLiteral("label_SlotTitle"));

        gridLayout->addWidget(label_SlotTitle, 0, 0, 1, 1);

        label_c1Sample = new QLabel(gridLayoutWidget);
        label_c1Sample->setObjectName(QStringLiteral("label_c1Sample"));
        label_c1Sample->setEnabled(false);

        gridLayout->addWidget(label_c1Sample, 1, 4, 1, 1);

        label_c2Sample = new QLabel(gridLayoutWidget);
        label_c2Sample->setObjectName(QStringLiteral("label_c2Sample"));
        label_c2Sample->setEnabled(false);

        gridLayout->addWidget(label_c2Sample, 2, 4, 1, 1);

        label_c3Sample = new QLabel(gridLayoutWidget);
        label_c3Sample->setObjectName(QStringLiteral("label_c3Sample"));
        label_c3Sample->setEnabled(false);

        gridLayout->addWidget(label_c3Sample, 3, 4, 1, 1);

        label_c4Sample = new QLabel(gridLayoutWidget);
        label_c4Sample->setObjectName(QStringLiteral("label_c4Sample"));
        label_c4Sample->setEnabled(false);

        gridLayout->addWidget(label_c4Sample, 4, 4, 1, 1);

        label_c5Sample = new QLabel(gridLayoutWidget);
        label_c5Sample->setObjectName(QStringLiteral("label_c5Sample"));
        label_c5Sample->setEnabled(false);

        gridLayout->addWidget(label_c5Sample, 5, 4, 1, 1);

        label_c6Sample = new QLabel(gridLayoutWidget);
        label_c6Sample->setObjectName(QStringLiteral("label_c6Sample"));
        label_c6Sample->setEnabled(false);

        gridLayout->addWidget(label_c6Sample, 6, 4, 1, 1);

        spinBox_c1R = new QSpinBox(gridLayoutWidget);
        spinBox_c1R->setObjectName(QStringLiteral("spinBox_c1R"));
        spinBox_c1R->setEnabled(false);
        spinBox_c1R->setMaximum(255);

        gridLayout->addWidget(spinBox_c1R, 1, 1, 1, 1);

        spinBox_c1G = new QSpinBox(gridLayoutWidget);
        spinBox_c1G->setObjectName(QStringLiteral("spinBox_c1G"));
        spinBox_c1G->setEnabled(false);
        spinBox_c1G->setMaximum(255);

        gridLayout->addWidget(spinBox_c1G, 1, 2, 1, 1);

        spinBox_c1B = new QSpinBox(gridLayoutWidget);
        spinBox_c1B->setObjectName(QStringLiteral("spinBox_c1B"));
        spinBox_c1B->setEnabled(false);
        spinBox_c1B->setMaximum(255);

        gridLayout->addWidget(spinBox_c1B, 1, 3, 1, 1);

        spinBox_c2R = new QSpinBox(gridLayoutWidget);
        spinBox_c2R->setObjectName(QStringLiteral("spinBox_c2R"));
        spinBox_c2R->setEnabled(false);
        spinBox_c2R->setMaximum(255);

        gridLayout->addWidget(spinBox_c2R, 2, 1, 1, 1);

        spinBox_c2G = new QSpinBox(gridLayoutWidget);
        spinBox_c2G->setObjectName(QStringLiteral("spinBox_c2G"));
        spinBox_c2G->setEnabled(false);
        spinBox_c2G->setMaximum(255);

        gridLayout->addWidget(spinBox_c2G, 2, 2, 1, 1);

        spinBox_c2B = new QSpinBox(gridLayoutWidget);
        spinBox_c2B->setObjectName(QStringLiteral("spinBox_c2B"));
        spinBox_c2B->setEnabled(false);
        spinBox_c2B->setMaximum(255);

        gridLayout->addWidget(spinBox_c2B, 2, 3, 1, 1);

        spinBox_c3R = new QSpinBox(gridLayoutWidget);
        spinBox_c3R->setObjectName(QStringLiteral("spinBox_c3R"));
        spinBox_c3R->setEnabled(false);
        spinBox_c3R->setMaximum(255);

        gridLayout->addWidget(spinBox_c3R, 3, 1, 1, 1);

        spinBox_c3G = new QSpinBox(gridLayoutWidget);
        spinBox_c3G->setObjectName(QStringLiteral("spinBox_c3G"));
        spinBox_c3G->setEnabled(false);
        spinBox_c3G->setMaximum(255);

        gridLayout->addWidget(spinBox_c3G, 3, 2, 1, 1);

        spinBox_c3B = new QSpinBox(gridLayoutWidget);
        spinBox_c3B->setObjectName(QStringLiteral("spinBox_c3B"));
        spinBox_c3B->setEnabled(false);
        spinBox_c3B->setMaximum(255);

        gridLayout->addWidget(spinBox_c3B, 3, 3, 1, 1);

        spinBox_c4R = new QSpinBox(gridLayoutWidget);
        spinBox_c4R->setObjectName(QStringLiteral("spinBox_c4R"));
        spinBox_c4R->setEnabled(false);
        spinBox_c4R->setMaximum(255);

        gridLayout->addWidget(spinBox_c4R, 4, 1, 1, 1);

        spinBox_c4G = new QSpinBox(gridLayoutWidget);
        spinBox_c4G->setObjectName(QStringLiteral("spinBox_c4G"));
        spinBox_c4G->setEnabled(false);
        spinBox_c4G->setMaximum(255);

        gridLayout->addWidget(spinBox_c4G, 4, 2, 1, 1);

        spinBox_c4B = new QSpinBox(gridLayoutWidget);
        spinBox_c4B->setObjectName(QStringLiteral("spinBox_c4B"));
        spinBox_c4B->setEnabled(false);
        spinBox_c4B->setMaximum(255);

        gridLayout->addWidget(spinBox_c4B, 4, 3, 1, 1);

        spinBox_c5R = new QSpinBox(gridLayoutWidget);
        spinBox_c5R->setObjectName(QStringLiteral("spinBox_c5R"));
        spinBox_c5R->setEnabled(false);
        spinBox_c5R->setMaximum(255);

        gridLayout->addWidget(spinBox_c5R, 5, 1, 1, 1);

        spinBox_c5G = new QSpinBox(gridLayoutWidget);
        spinBox_c5G->setObjectName(QStringLiteral("spinBox_c5G"));
        spinBox_c5G->setEnabled(false);
        spinBox_c5G->setMaximum(255);

        gridLayout->addWidget(spinBox_c5G, 5, 2, 1, 1);

        spinBox_c5B = new QSpinBox(gridLayoutWidget);
        spinBox_c5B->setObjectName(QStringLiteral("spinBox_c5B"));
        spinBox_c5B->setEnabled(false);
        spinBox_c5B->setMaximum(255);

        gridLayout->addWidget(spinBox_c5B, 5, 3, 1, 1);

        spinBox_c6R = new QSpinBox(gridLayoutWidget);
        spinBox_c6R->setObjectName(QStringLiteral("spinBox_c6R"));
        spinBox_c6R->setEnabled(false);
        spinBox_c6R->setMaximum(255);

        gridLayout->addWidget(spinBox_c6R, 6, 1, 1, 1);

        spinBox_c6G = new QSpinBox(gridLayoutWidget);
        spinBox_c6G->setObjectName(QStringLiteral("spinBox_c6G"));
        spinBox_c6G->setEnabled(false);
        spinBox_c6G->setMaximum(255);

        gridLayout->addWidget(spinBox_c6G, 6, 2, 1, 1);

        spinBox_c6B = new QSpinBox(gridLayoutWidget);
        spinBox_c6B->setObjectName(QStringLiteral("spinBox_c6B"));
        spinBox_c6B->setEnabled(false);
        spinBox_c6B->setMaximum(255);

        gridLayout->addWidget(spinBox_c6B, 6, 3, 1, 1);

        checkBox_c8 = new QCheckBox(gridLayoutWidget);
        checkBox_c8->setObjectName(QStringLiteral("checkBox_c8"));

        gridLayout->addWidget(checkBox_c8, 8, 0, 1, 1);

        spinBox_c8B = new QSpinBox(gridLayoutWidget);
        spinBox_c8B->setObjectName(QStringLiteral("spinBox_c8B"));
        spinBox_c8B->setEnabled(false);
        spinBox_c8B->setMaximum(255);

        gridLayout->addWidget(spinBox_c8B, 8, 3, 1, 1);

        label_c7Sample = new QLabel(gridLayoutWidget);
        label_c7Sample->setObjectName(QStringLiteral("label_c7Sample"));
        label_c7Sample->setEnabled(false);

        gridLayout->addWidget(label_c7Sample, 7, 4, 1, 1);

        label_c8Sample = new QLabel(gridLayoutWidget);
        label_c8Sample->setObjectName(QStringLiteral("label_c8Sample"));
        label_c8Sample->setEnabled(false);

        gridLayout->addWidget(label_c8Sample, 8, 4, 1, 1);

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

        retranslateUi(ABBHelper);

        QMetaObject::connectSlotsByName(ABBHelper);
    } // setupUi

    void retranslateUi(QWidget *ABBHelper)
    {
        ABBHelper->setWindowTitle(QApplication::translate("ABBHelper", "ABB Parameters", 0));
        checkBox_c7->setText(QApplication::translate("ABBHelper", "Color G", 0));
        label_BlueTitle->setText(QApplication::translate("ABBHelper", "Blue", 0));
        label_SampleTitle->setText(QApplication::translate("ABBHelper", "Sample", 0));
        label_GreenTitle->setText(QApplication::translate("ABBHelper", "Green", 0));
        label_RedTitle->setText(QApplication::translate("ABBHelper", "Red", 0));
        checkBox_c2->setText(QApplication::translate("ABBHelper", "Color B", 0));
        checkBox_c4->setText(QApplication::translate("ABBHelper", "Color D", 0));
        checkBox_c6->setText(QApplication::translate("ABBHelper", "Color F", 0));
        checkBox_c1->setText(QApplication::translate("ABBHelper", "Color A", 0));
        checkBox_c5->setText(QApplication::translate("ABBHelper", "Color E", 0));
        checkBox_c3->setText(QApplication::translate("ABBHelper", "Color C", 0));
        label_SlotTitle->setText(QApplication::translate("ABBHelper", "Slot In Use", 0));
        label_c1Sample->setText(QString());
        label_c2Sample->setText(QString());
        label_c3Sample->setText(QString());
        label_c4Sample->setText(QString());
        label_c5Sample->setText(QString());
        label_c6Sample->setText(QString());
        checkBox_c8->setText(QApplication::translate("ABBHelper", "Color H", 0));
        label_c7Sample->setText(QString());
        label_c8Sample->setText(QString());
        label_Port->setText(QApplication::translate("ABBHelper", "<html><head/><body><p><span style=\" font-size:10pt;\">Port:</span></p></body></html>", 0));
        spinBox_comNum->setPrefix(QApplication::translate("ABBHelper", "COM", 0));
    } // retranslateUi

};

namespace Ui {
    class ABBHelper: public Ui_ABBHelper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABBHELPER_H
