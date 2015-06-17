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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Painter
{
public:
    QLabel *label;
    QFrame *line;

    void setupUi(QWidget *Painter)
    {
        if (Painter->objectName().isEmpty())
            Painter->setObjectName(QStringLiteral("Painter"));
        Painter->resize(1040, 800);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Painter->sizePolicy().hasHeightForWidth());
        Painter->setSizePolicy(sizePolicy);
        label = new QLabel(Painter);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 1000, 750));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        line = new QFrame(Painter);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 30, 1041, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

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
