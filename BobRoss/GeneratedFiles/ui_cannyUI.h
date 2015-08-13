/********************************************************************************
** Form generated from reading UI file 'cannyUI.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
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
#include <QtWidgets/QDialogButtonBox>
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
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *ThresholdLabel;
    QLabel *MinLengthLabel;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *ThresholdInput;
    QLineEdit *LengthInput;
    QLabel *ImageLabel;
    QLineEdit *ImageInput;
    QPushButton *browse;

    void setupUi(QDialog *canny)
    {
        if (canny->objectName().isEmpty())
            canny->setObjectName(QStringLiteral("canny"));
        canny->resize(279, 184);
        buttonBox = new QDialogButtonBox(canny);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(-100, 140, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget = new QWidget(canny);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 10, 266, 80));
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


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        ThresholdInput = new QLineEdit(horizontalLayoutWidget);
        ThresholdInput->setObjectName(QStringLiteral("ThresholdInput"));

        verticalLayout_2->addWidget(ThresholdInput);

        LengthInput = new QLineEdit(horizontalLayoutWidget);
        LengthInput->setObjectName(QStringLiteral("LengthInput"));

        verticalLayout_2->addWidget(LengthInput);


        horizontalLayout->addLayout(verticalLayout_2);

        ImageLabel = new QLabel(canny);
        ImageLabel->setObjectName(QStringLiteral("ImageLabel"));
        ImageLabel->setGeometry(QRect(20, 90, 51, 34));
        ImageInput = new QLineEdit(canny);
        ImageInput->setObjectName(QStringLiteral("ImageInput"));
        ImageInput->setGeometry(QRect(70, 100, 161, 22));
        browse = new QPushButton(canny);
        browse->setObjectName(QStringLiteral("browse"));
        browse->setGeometry(QRect(240, 100, 31, 21));

        retranslateUi(canny);
        QObject::connect(buttonBox, SIGNAL(accepted()), canny, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), canny, SLOT(reject()));

        QMetaObject::connectSlotsByName(canny);
    } // setupUi

    void retranslateUi(QDialog *canny)
    {
        canny->setWindowTitle(QApplication::translate("canny", "Dialog", 0));
        ThresholdLabel->setText(QApplication::translate("canny", "Threshold (%)", 0));
        MinLengthLabel->setText(QApplication::translate("canny", "Minimum Line Length (pixels)", 0));
        ThresholdInput->setText(QApplication::translate("canny", "50", 0));
        LengthInput->setText(QApplication::translate("canny", "5", 0));
        ImageLabel->setText(QApplication::translate("canny", "Image:", 0));
        ImageInput->setText(QString());
        browse->setText(QApplication::translate("canny", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class canny: public Ui_canny {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANNYUI_H
