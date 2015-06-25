/********************************************************************************
** Form generated from reading UI file 'painter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTER_H
#define UI_PAINTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Painter
{
public:
    QLabel *label;

    void setupUi(QWidget *Painter)
    {
        if (Painter->objectName().isEmpty())
            Painter->setObjectName(QStringLiteral("Painter"));
        Painter->resize(1015, 765);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Painter->sizePolicy().hasHeightForWidth());
        Painter->setSizePolicy(sizePolicy);
        label = new QLabel(Painter);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 1000, 750));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setIndent(0);

        retranslateUi(Painter);

        QMetaObject::connectSlotsByName(Painter);
    } // setupUi

    void retranslateUi(QWidget *Painter)
    {
        Painter->setWindowTitle(QApplication::translate("Painter", "Form", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Painter: public Ui_Painter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTER_H
