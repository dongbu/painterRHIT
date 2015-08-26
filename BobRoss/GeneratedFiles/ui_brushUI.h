/********************************************************************************
** Form generated from reading UI file 'brushUI.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRUSHUI_H
#define UI_BRUSHUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_brush
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *shapeInput;
    QHBoxLayout *horizontalLayout;
    QLabel *widthLabel;
    QLineEdit *widthInput;
    QHBoxLayout *horizontalLayout_2;
    QLabel *heightLabel;
    QLineEdit *heightInput;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QPushButton *cancel;
    QPushButton *accept;

    void setupUi(QDialog *brush)
    {
        if (brush->objectName().isEmpty())
            brush->setObjectName(QStringLiteral("brush"));
        brush->resize(219, 130);
        layoutWidget = new QWidget(brush);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 111, 113));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        shapeInput = new QComboBox(layoutWidget);
        shapeInput->setObjectName(QStringLiteral("shapeInput"));

        horizontalLayout_3->addWidget(shapeInput);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widthLabel = new QLabel(layoutWidget);
        widthLabel->setObjectName(QStringLiteral("widthLabel"));

        horizontalLayout->addWidget(widthLabel);

        widthInput = new QLineEdit(layoutWidget);
        widthInput->setObjectName(QStringLiteral("widthInput"));

        horizontalLayout->addWidget(widthInput);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        heightLabel = new QLabel(layoutWidget);
        heightLabel->setObjectName(QStringLiteral("heightLabel"));

        horizontalLayout_2->addWidget(heightLabel);

        heightInput = new QLineEdit(layoutWidget);
        heightInput->setObjectName(QStringLiteral("heightInput"));

        horizontalLayout_2->addWidget(heightInput);


        verticalLayout->addLayout(horizontalLayout_2);

        layoutWidget1 = new QWidget(brush);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(120, 10, 95, 81));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        cancel = new QPushButton(layoutWidget1);
        cancel->setObjectName(QStringLiteral("cancel"));

        verticalLayout_2->addWidget(cancel);

        accept = new QPushButton(layoutWidget1);
        accept->setObjectName(QStringLiteral("accept"));

        verticalLayout_2->addWidget(accept);


        retranslateUi(brush);

        QMetaObject::connectSlotsByName(brush);
    } // setupUi

    void retranslateUi(QDialog *brush)
    {
        brush->setWindowTitle(QApplication::translate("brush", "Dialog", 0));
        shapeInput->clear();
        shapeInput->insertItems(0, QStringList()
         << QApplication::translate("brush", "Rectangle", 0)
         << QApplication::translate("brush", "Ellipse", 0)
        );
        widthLabel->setText(QApplication::translate("brush", "Width:", 0));
        widthInput->setText(QApplication::translate("brush", "30", 0));
        heightLabel->setText(QApplication::translate("brush", "Height:", 0));
        heightInput->setText(QApplication::translate("brush", "15", 0));
        cancel->setText(QApplication::translate("brush", "Cancel", 0));
        accept->setText(QApplication::translate("brush", "Accept", 0));
    } // retranslateUi

};

namespace Ui {
    class brush: public Ui_brush {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRUSHUI_H
