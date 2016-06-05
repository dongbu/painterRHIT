/********************************************************************************
** Form generated from reading UI file 'cannyUI.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANNYUI_H
#define UI_CANNYUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_canny
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *ThresholdLabel;
    QLabel *MinLengthLabel;
    QLabel *SkipLabel;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *ThresholdInput;
    QLineEdit *LengthInput;
    QLineEdit *SkipInput;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cancel;
    QPushButton *accept;

    void setupUi(QDialog *canny)
    {
        if (canny->objectName().isEmpty())
            canny->setObjectName(QStringLiteral("canny"));
        canny->resize(278, 147);
        horizontalLayoutWidget = new QWidget(canny);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 10, 266, 81));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ThresholdLabel = new QLabel(horizontalLayoutWidget);
        ThresholdLabel->setObjectName(QStringLiteral("ThresholdLabel"));

        verticalLayout->addWidget(ThresholdLabel);

        MinLengthLabel = new QLabel(horizontalLayoutWidget);
        MinLengthLabel->setObjectName(QStringLiteral("MinLengthLabel"));

        verticalLayout->addWidget(MinLengthLabel);

        SkipLabel = new QLabel(horizontalLayoutWidget);
        SkipLabel->setObjectName(QStringLiteral("SkipLabel"));

        verticalLayout->addWidget(SkipLabel);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        ThresholdInput = new QLineEdit(horizontalLayoutWidget);
        ThresholdInput->setObjectName(QStringLiteral("ThresholdInput"));

        verticalLayout_2->addWidget(ThresholdInput);

        LengthInput = new QLineEdit(horizontalLayoutWidget);
        LengthInput->setObjectName(QStringLiteral("LengthInput"));

        verticalLayout_2->addWidget(LengthInput);

        SkipInput = new QLineEdit(horizontalLayoutWidget);
        SkipInput->setObjectName(QStringLiteral("SkipInput"));

        verticalLayout_2->addWidget(SkipInput);


        horizontalLayout->addLayout(verticalLayout_2);

        layoutWidget = new QWidget(canny);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 100, 261, 30));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        cancel = new QPushButton(layoutWidget);
        cancel->setObjectName(QStringLiteral("cancel"));

        horizontalLayout_2->addWidget(cancel);

        accept = new QPushButton(layoutWidget);
        accept->setObjectName(QStringLiteral("accept"));

        horizontalLayout_2->addWidget(accept);


        retranslateUi(canny);

        accept->setDefault(true);


        QMetaObject::connectSlotsByName(canny);
    } // setupUi

    void retranslateUi(QDialog *canny)
    {
        canny->setWindowTitle(QApplication::translate("canny", "Dialog", 0));
        ThresholdLabel->setText(QApplication::translate("canny", "Threshold (%)", 0));
        MinLengthLabel->setText(QApplication::translate("canny", "Minimum Line Length (pixels)", 0));
        SkipLabel->setText(QApplication::translate("canny", "Skip Frequency", 0));
        ThresholdInput->setText(QApplication::translate("canny", "50", 0));
        LengthInput->setText(QApplication::translate("canny", "5", 0));
        SkipInput->setText(QApplication::translate("canny", "1", 0));
        cancel->setText(QApplication::translate("canny", "Cancel", 0));
        accept->setText(QApplication::translate("canny", "Accept", 0));
    } // retranslateUi

};

namespace Ui {
    class canny: public Ui_canny {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANNYUI_H
