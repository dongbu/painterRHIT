/********************************************************************************
** Form generated from reading UI file 'CytonRunner.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CYTONRUNNER_H
#define UI_CYTONRUNNER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_5;
    QLabel *label_10;
    QDoubleSpinBox *doubleSpinBox_6;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_7;
    QLabel *label_12;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_25;
    QLabel *label_27;
    QLineEdit *lineEdit_14;
    QLabel *label_28;
    QLineEdit *lineEdit_15;
    QLabel *label_29;
    QLineEdit *lineEdit_17;
    QLabel *label_30;
    QLineEdit *lineEdit_16;
    QLabel *label_31;
    QLabel *label_26;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_38;
    QLabel *label_36;
    QLabel *label_35;
    QLabel *label_32;
    QLabel *label_39;
    QLabel *label_37;
    QLabel *label_41;
    QLabel *label_42;
    QLabel *label_33;
    QLineEdit *lineEdit_20;
    QLineEdit *lineEdit_19;
    QLabel *label_47;
    QLabel *label_48;
    QLineEdit *lineEdit_24;
    QLineEdit *lineEdit_25;
    QLineEdit *lineEdit_26;
    QLineEdit *lineEdit_27;
    QLineEdit *lineEdit_28;
    QFrame *line_3;
    QLineEdit *lineEdit_29;
    QLineEdit *lineEdit_23;
    QLabel *label_44;
    QLabel *label_46;
    QFrame *line_2;
    QFrame *line;
    QLabel *label_45;
    QLineEdit *lineEdit_22;
    QLabel *label_43;
    QLineEdit *lineEdit_21;
    QLineEdit *lineEdit_18;
    QLabel *label_34;
    QLabel *label_40;
    QFrame *line_4;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_40;
    QLineEdit *lineEdit_37;
    QLineEdit *lineEdit_33;
    QLineEdit *lineEdit_31;
    QLineEdit *lineEdit_42;
    QLineEdit *lineEdit_39;
    QLineEdit *lineEdit_43;
    QLineEdit *lineEdit_44;
    QLabel *label_11;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_41;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_34;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_32;
    QLineEdit *lineEdit_36;
    QLineEdit *lineEdit_35;
    QLineEdit *lineEdit_38;
    QLineEdit *lineEdit_30;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(979, 592);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(50, 500, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(Dialog);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 10, 281, 261));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(formLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, label_3);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        doubleSpinBox = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setMinimum(-2.62);
        doubleSpinBox->setMaximum(2.62);
        doubleSpinBox->setValue(-2);

        formLayout->setWidget(3, QFormLayout::FieldRole, doubleSpinBox);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        doubleSpinBox_2 = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setMinimum(-1.83);
        doubleSpinBox_2->setMaximum(1.83);
        doubleSpinBox_2->setValue(-1);

        formLayout->setWidget(4, QFormLayout::FieldRole, doubleSpinBox_2);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_7);

        doubleSpinBox_3 = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setMinimum(-1.83);
        doubleSpinBox_3->setMaximum(1.83);

        formLayout->setWidget(5, QFormLayout::FieldRole, doubleSpinBox_3);

        label_8 = new QLabel(formLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_8);

        doubleSpinBox_4 = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));
        doubleSpinBox_4->setMinimum(-1.83);
        doubleSpinBox_4->setValue(-0.5);

        formLayout->setWidget(6, QFormLayout::FieldRole, doubleSpinBox_4);

        label_9 = new QLabel(formLayoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_9);

        doubleSpinBox_5 = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox_5->setObjectName(QStringLiteral("doubleSpinBox_5"));
        doubleSpinBox_5->setMinimum(-1.83);
        doubleSpinBox_5->setValue(-1.1);

        formLayout->setWidget(7, QFormLayout::FieldRole, doubleSpinBox_5);

        label_10 = new QLabel(formLayoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_10);

        doubleSpinBox_6 = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox_6->setObjectName(QStringLiteral("doubleSpinBox_6"));
        doubleSpinBox_6->setMinimum(-1.83);
        doubleSpinBox_6->setValue(-1.2);

        formLayout->setWidget(8, QFormLayout::FieldRole, doubleSpinBox_6);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_6);

        doubleSpinBox_7 = new QDoubleSpinBox(formLayoutWidget);
        doubleSpinBox_7->setObjectName(QStringLiteral("doubleSpinBox_7"));
        doubleSpinBox_7->setDecimals(3);
        doubleSpinBox_7->setMinimum(-2.61);
        doubleSpinBox_7->setMaximum(-2.61);
        doubleSpinBox_7->setValue(-2.61);

        formLayout->setWidget(9, QFormLayout::FieldRole, doubleSpinBox_7);

        label_12 = new QLabel(formLayoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout->setWidget(10, QFormLayout::LabelRole, label_12);

        formLayoutWidget_2 = new QWidget(Dialog);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(20, 280, 281, 163));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label_25 = new QLabel(formLayoutWidget_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_25);

        label_27 = new QLabel(formLayoutWidget_2);
        label_27->setObjectName(QStringLiteral("label_27"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_27);

        lineEdit_14 = new QLineEdit(formLayoutWidget_2);
        lineEdit_14->setObjectName(QStringLiteral("lineEdit_14"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit_14);

        label_28 = new QLabel(formLayoutWidget_2);
        label_28->setObjectName(QStringLiteral("label_28"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_28);

        lineEdit_15 = new QLineEdit(formLayoutWidget_2);
        lineEdit_15->setObjectName(QStringLiteral("lineEdit_15"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_15);

        label_29 = new QLabel(formLayoutWidget_2);
        label_29->setObjectName(QStringLiteral("label_29"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_29);

        lineEdit_17 = new QLineEdit(formLayoutWidget_2);
        lineEdit_17->setObjectName(QStringLiteral("lineEdit_17"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, lineEdit_17);

        label_30 = new QLabel(formLayoutWidget_2);
        label_30->setObjectName(QStringLiteral("label_30"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_30);

        lineEdit_16 = new QLineEdit(formLayoutWidget_2);
        lineEdit_16->setObjectName(QStringLiteral("lineEdit_16"));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, lineEdit_16);

        label_31 = new QLabel(formLayoutWidget_2);
        label_31->setObjectName(QStringLiteral("label_31"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_31);

        label_26 = new QLabel(formLayoutWidget_2);
        label_26->setObjectName(QStringLiteral("label_26"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_26);

        gridLayoutWidget = new QWidget(Dialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(320, 10, 341, 571));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_38 = new QLabel(gridLayoutWidget);
        label_38->setObjectName(QStringLiteral("label_38"));

        gridLayout->addWidget(label_38, 8, 0, 1, 1);

        label_36 = new QLabel(gridLayoutWidget);
        label_36->setObjectName(QStringLiteral("label_36"));

        gridLayout->addWidget(label_36, 5, 0, 1, 1);

        label_35 = new QLabel(gridLayoutWidget);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout->addWidget(label_35, 4, 0, 1, 1);

        label_32 = new QLabel(gridLayoutWidget);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout->addWidget(label_32, 0, 0, 1, 2);

        label_39 = new QLabel(gridLayoutWidget);
        label_39->setObjectName(QStringLiteral("label_39"));

        gridLayout->addWidget(label_39, 9, 0, 1, 1);

        label_37 = new QLabel(gridLayoutWidget);
        label_37->setObjectName(QStringLiteral("label_37"));

        gridLayout->addWidget(label_37, 7, 0, 1, 2);

        label_41 = new QLabel(gridLayoutWidget);
        label_41->setObjectName(QStringLiteral("label_41"));

        gridLayout->addWidget(label_41, 12, 0, 1, 2);

        label_42 = new QLabel(gridLayoutWidget);
        label_42->setObjectName(QStringLiteral("label_42"));

        gridLayout->addWidget(label_42, 13, 0, 1, 1);

        label_33 = new QLabel(gridLayoutWidget);
        label_33->setObjectName(QStringLiteral("label_33"));

        gridLayout->addWidget(label_33, 2, 0, 1, 2);

        lineEdit_20 = new QLineEdit(gridLayoutWidget);
        lineEdit_20->setObjectName(QStringLiteral("lineEdit_20"));

        gridLayout->addWidget(lineEdit_20, 5, 1, 1, 1);

        lineEdit_19 = new QLineEdit(gridLayoutWidget);
        lineEdit_19->setObjectName(QStringLiteral("lineEdit_19"));

        gridLayout->addWidget(lineEdit_19, 4, 1, 1, 1);

        label_47 = new QLabel(gridLayoutWidget);
        label_47->setObjectName(QStringLiteral("label_47"));

        gridLayout->addWidget(label_47, 19, 0, 1, 1);

        label_48 = new QLabel(gridLayoutWidget);
        label_48->setObjectName(QStringLiteral("label_48"));

        gridLayout->addWidget(label_48, 20, 0, 1, 1);

        lineEdit_24 = new QLineEdit(gridLayoutWidget);
        lineEdit_24->setObjectName(QStringLiteral("lineEdit_24"));

        gridLayout->addWidget(lineEdit_24, 13, 1, 1, 1);

        lineEdit_25 = new QLineEdit(gridLayoutWidget);
        lineEdit_25->setObjectName(QStringLiteral("lineEdit_25"));

        gridLayout->addWidget(lineEdit_25, 14, 1, 1, 1);

        lineEdit_26 = new QLineEdit(gridLayoutWidget);
        lineEdit_26->setObjectName(QStringLiteral("lineEdit_26"));

        gridLayout->addWidget(lineEdit_26, 15, 1, 1, 1);

        lineEdit_27 = new QLineEdit(gridLayoutWidget);
        lineEdit_27->setObjectName(QStringLiteral("lineEdit_27"));

        gridLayout->addWidget(lineEdit_27, 18, 1, 1, 1);

        lineEdit_28 = new QLineEdit(gridLayoutWidget);
        lineEdit_28->setObjectName(QStringLiteral("lineEdit_28"));

        gridLayout->addWidget(lineEdit_28, 19, 1, 1, 1);

        line_3 = new QFrame(gridLayoutWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 11, 0, 1, 2);

        lineEdit_29 = new QLineEdit(gridLayoutWidget);
        lineEdit_29->setObjectName(QStringLiteral("lineEdit_29"));

        gridLayout->addWidget(lineEdit_29, 20, 1, 1, 1);

        lineEdit_23 = new QLineEdit(gridLayoutWidget);
        lineEdit_23->setObjectName(QStringLiteral("lineEdit_23"));

        gridLayout->addWidget(lineEdit_23, 10, 1, 1, 1);

        label_44 = new QLabel(gridLayoutWidget);
        label_44->setObjectName(QStringLiteral("label_44"));

        gridLayout->addWidget(label_44, 15, 0, 1, 1);

        label_46 = new QLabel(gridLayoutWidget);
        label_46->setObjectName(QStringLiteral("label_46"));

        gridLayout->addWidget(label_46, 18, 0, 1, 1);

        line_2 = new QFrame(gridLayoutWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 6, 0, 1, 2);

        line = new QFrame(gridLayoutWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 2);

        label_45 = new QLabel(gridLayoutWidget);
        label_45->setObjectName(QStringLiteral("label_45"));

        gridLayout->addWidget(label_45, 17, 0, 1, 2);

        lineEdit_22 = new QLineEdit(gridLayoutWidget);
        lineEdit_22->setObjectName(QStringLiteral("lineEdit_22"));

        gridLayout->addWidget(lineEdit_22, 9, 1, 1, 1);

        label_43 = new QLabel(gridLayoutWidget);
        label_43->setObjectName(QStringLiteral("label_43"));

        gridLayout->addWidget(label_43, 14, 0, 1, 1);

        lineEdit_21 = new QLineEdit(gridLayoutWidget);
        lineEdit_21->setObjectName(QStringLiteral("lineEdit_21"));

        gridLayout->addWidget(lineEdit_21, 8, 1, 1, 1);

        lineEdit_18 = new QLineEdit(gridLayoutWidget);
        lineEdit_18->setObjectName(QStringLiteral("lineEdit_18"));

        gridLayout->addWidget(lineEdit_18, 3, 1, 1, 1);

        label_34 = new QLabel(gridLayoutWidget);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout->addWidget(label_34, 3, 0, 1, 1);

        label_40 = new QLabel(gridLayoutWidget);
        label_40->setObjectName(QStringLiteral("label_40"));

        gridLayout->addWidget(label_40, 10, 0, 1, 1);

        line_4 = new QFrame(gridLayoutWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_4, 16, 0, 1, 2);

        gridLayoutWidget_2 = new QWidget(Dialog);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(680, 10, 291, 571));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit_40 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_40->setObjectName(QStringLiteral("lineEdit_40"));

        gridLayout_2->addWidget(lineEdit_40, 10, 1, 1, 1);

        lineEdit_37 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_37->setObjectName(QStringLiteral("lineEdit_37"));

        gridLayout_2->addWidget(lineEdit_37, 8, 1, 1, 1);

        lineEdit_33 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_33->setObjectName(QStringLiteral("lineEdit_33"));

        gridLayout_2->addWidget(lineEdit_33, 7, 1, 1, 1);

        lineEdit_31 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_31->setObjectName(QStringLiteral("lineEdit_31"));

        gridLayout_2->addWidget(lineEdit_31, 8, 2, 1, 1);

        lineEdit_42 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_42->setObjectName(QStringLiteral("lineEdit_42"));

        gridLayout_2->addWidget(lineEdit_42, 7, 0, 1, 1);

        lineEdit_39 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_39->setObjectName(QStringLiteral("lineEdit_39"));

        gridLayout_2->addWidget(lineEdit_39, 10, 2, 1, 1);

        lineEdit_43 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_43->setObjectName(QStringLiteral("lineEdit_43"));

        gridLayout_2->addWidget(lineEdit_43, 8, 0, 1, 1);

        lineEdit_44 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_44->setObjectName(QStringLiteral("lineEdit_44"));

        gridLayout_2->addWidget(lineEdit_44, 10, 0, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 0, 0, 1, 3);

        lineEdit_9 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_9->setObjectName(QStringLiteral("lineEdit_9"));

        gridLayout_2->addWidget(lineEdit_9, 4, 2, 1, 1);

        lineEdit_10 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_10->setObjectName(QStringLiteral("lineEdit_10"));

        gridLayout_2->addWidget(lineEdit_10, 5, 1, 1, 1);

        lineEdit_11 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_11->setObjectName(QStringLiteral("lineEdit_11"));

        gridLayout_2->addWidget(lineEdit_11, 5, 2, 1, 1);

        lineEdit_12 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_12->setObjectName(QStringLiteral("lineEdit_12"));

        gridLayout_2->addWidget(lineEdit_12, 6, 1, 1, 1);

        lineEdit_13 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_13->setObjectName(QStringLiteral("lineEdit_13"));

        gridLayout_2->addWidget(lineEdit_13, 6, 2, 1, 1);

        lineEdit_41 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_41->setObjectName(QStringLiteral("lineEdit_41"));

        gridLayout_2->addWidget(lineEdit_41, 7, 2, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 1, 0, 1, 1);

        label_14 = new QLabel(gridLayoutWidget_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 1, 1, 1, 1);

        label_15 = new QLabel(gridLayoutWidget_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 1, 2, 1, 1);

        lineEdit_2 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        gridLayout_2->addWidget(lineEdit_2, 2, 0, 1, 1);

        lineEdit_3 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        gridLayout_2->addWidget(lineEdit_3, 2, 1, 1, 1);

        lineEdit_4 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        gridLayout_2->addWidget(lineEdit_4, 2, 2, 1, 1);

        lineEdit_7 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));

        gridLayout_2->addWidget(lineEdit_7, 6, 0, 1, 1);

        lineEdit_6 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));

        gridLayout_2->addWidget(lineEdit_6, 5, 0, 1, 1);

        lineEdit_5 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));

        gridLayout_2->addWidget(lineEdit_5, 4, 0, 1, 1);

        lineEdit_34 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_34->setObjectName(QStringLiteral("lineEdit_34"));

        gridLayout_2->addWidget(lineEdit_34, 17, 1, 1, 1);

        lineEdit_8 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));

        gridLayout_2->addWidget(lineEdit_8, 4, 1, 1, 1);

        lineEdit_32 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_32->setObjectName(QStringLiteral("lineEdit_32"));

        gridLayout_2->addWidget(lineEdit_32, 11, 2, 1, 1);

        lineEdit_36 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_36->setObjectName(QStringLiteral("lineEdit_36"));

        gridLayout_2->addWidget(lineEdit_36, 11, 1, 1, 1);

        lineEdit_35 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_35->setObjectName(QStringLiteral("lineEdit_35"));

        gridLayout_2->addWidget(lineEdit_35, 11, 0, 1, 1);

        lineEdit_38 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_38->setObjectName(QStringLiteral("lineEdit_38"));

        gridLayout_2->addWidget(lineEdit_38, 17, 0, 1, 1);

        lineEdit_30 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_30->setObjectName(QStringLiteral("lineEdit_30"));

        gridLayout_2->addWidget(lineEdit_30, 17, 2, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        label->setText(QApplication::translate("Dialog", "File Location", 0));
        lineEdit->setText(QApplication::translate("Dialog", "documents/example.xml", 0));
        label_2->setText(QString());
        label_3->setText(QApplication::translate("Dialog", "Starting Position", 0));
        label_4->setText(QApplication::translate("Dialog", "Shoulder Roll", 0));
        label_5->setText(QApplication::translate("Dialog", "Shoulder Pitch", 0));
        label_7->setText(QApplication::translate("Dialog", "Shoulder Yaw", 0));
        label_8->setText(QApplication::translate("Dialog", "Elbow Pitch", 0));
        label_9->setText(QApplication::translate("Dialog", "Elbow Yaw", 0));
        label_10->setText(QApplication::translate("Dialog", "Wrist Pitch", 0));
        label_6->setText(QApplication::translate("Dialog", "Wrist Roll", 0));
        label_12->setText(QString());
        label_25->setText(QString());
        label_27->setText(QApplication::translate("Dialog", "width (x)", 0));
        label_28->setText(QApplication::translate("Dialog", "width (y)", 0));
        label_29->setText(QApplication::translate("Dialog", "height (z)", 0));
        label_30->setText(QApplication::translate("Dialog", "type", 0));
        lineEdit_16->setText(QApplication::translate("Dialog", "(round or square)", 0));
        label_31->setText(QString());
        label_26->setText(QApplication::translate("Dialog", "Brush", 0));
        label_38->setText(QApplication::translate("Dialog", "X: ", 0));
        label_36->setText(QApplication::translate("Dialog", "Z: ", 0));
        label_35->setText(QApplication::translate("Dialog", "Y:", 0));
        label_32->setText(QApplication::translate("Dialog", "Canvas Corners", 0));
        label_39->setText(QApplication::translate("Dialog", "Y:", 0));
        label_37->setText(QApplication::translate("Dialog", "top-right", 0));
        label_41->setText(QApplication::translate("Dialog", "bottom-left", 0));
        label_42->setText(QApplication::translate("Dialog", "X:", 0));
        label_33->setText(QApplication::translate("Dialog", "top-left", 0));
        label_47->setText(QApplication::translate("Dialog", "Y:", 0));
        label_48->setText(QApplication::translate("Dialog", "Z:", 0));
        label_44->setText(QApplication::translate("Dialog", "Z:", 0));
        label_46->setText(QApplication::translate("Dialog", "X:", 0));
        label_45->setText(QApplication::translate("Dialog", "bottom-right", 0));
        label_43->setText(QApplication::translate("Dialog", "Y:", 0));
        label_34->setText(QApplication::translate("Dialog", "X: ", 0));
        label_40->setText(QApplication::translate("Dialog", "Y:", 0));
        label_11->setText(QApplication::translate("Dialog", "Paint Can Locations", 0));
        label_13->setText(QApplication::translate("Dialog", "ID", 0));
        label_14->setText(QApplication::translate("Dialog", "X position", 0));
        label_15->setText(QApplication::translate("Dialog", "Y position", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CYTONRUNNER_H
