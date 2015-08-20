/********************************************************************************
** Form generated from reading UI file 'kMeansUI.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KMEANSUI_H
#define UI_KMEANSUI_H

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

class Ui_kMeans
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *ColorLabel;
    QLabel *MinSizeLabel;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *ColorInput;
    QLineEdit *SizeInput;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cancel;
    QPushButton *accept;

    void setupUi(QDialog *kMeans)
    {
        if (kMeans->objectName().isEmpty())
            kMeans->setObjectName(QStringLiteral("kMeans"));
        kMeans->resize(267, 138);
        horizontalLayoutWidget = new QWidget(kMeans);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 251, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ColorLabel = new QLabel(horizontalLayoutWidget);
        ColorLabel->setObjectName(QStringLiteral("ColorLabel"));

        verticalLayout->addWidget(ColorLabel);

        MinSizeLabel = new QLabel(horizontalLayoutWidget);
        MinSizeLabel->setObjectName(QStringLiteral("MinSizeLabel"));

        verticalLayout->addWidget(MinSizeLabel);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        ColorInput = new QLineEdit(horizontalLayoutWidget);
        ColorInput->setObjectName(QStringLiteral("ColorInput"));

        verticalLayout_2->addWidget(ColorInput);

        SizeInput = new QLineEdit(horizontalLayoutWidget);
        SizeInput->setObjectName(QStringLiteral("SizeInput"));

        verticalLayout_2->addWidget(SizeInput);


        horizontalLayout->addLayout(verticalLayout_2);

        layoutWidget = new QWidget(kMeans);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 100, 251, 30));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        cancel = new QPushButton(layoutWidget);
        cancel->setObjectName(QStringLiteral("cancel"));

        horizontalLayout_2->addWidget(cancel);

        accept = new QPushButton(layoutWidget);
        accept->setObjectName(QStringLiteral("accept"));

        horizontalLayout_2->addWidget(accept);


        retranslateUi(kMeans);

        QMetaObject::connectSlotsByName(kMeans);
    } // setupUi

    void retranslateUi(QDialog *kMeans)
    {
        kMeans->setWindowTitle(QApplication::translate("kMeans", "Dialog", 0));
        ColorLabel->setText(QApplication::translate("kMeans", "Color Count", 0));
        MinSizeLabel->setText(QApplication::translate("kMeans", "Minimum Region Size (pixels)", 0));
        ColorInput->setText(QApplication::translate("kMeans", "2", 0));
        SizeInput->setText(QApplication::translate("kMeans", "5", 0));
        cancel->setText(QApplication::translate("kMeans", "Cancel", 0));
        accept->setText(QApplication::translate("kMeans", "Accept", 0));
    } // retranslateUi

};

namespace Ui {
    class kMeans: public Ui_kMeans {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KMEANSUI_H
