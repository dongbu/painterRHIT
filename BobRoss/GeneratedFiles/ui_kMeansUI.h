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
#include <QtWidgets/QDialogButtonBox>
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
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *ColorLabel;
    QLabel *MinSizeLabel;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *ColorInput;
    QLineEdit *SizeInput;
    QLabel *ImageLabel;
    QLineEdit *ImageInput;
    QPushButton *browse;

    void setupUi(QDialog *kMeans)
    {
        if (kMeans->objectName().isEmpty())
            kMeans->setObjectName(QStringLiteral("kMeans"));
        kMeans->resize(279, 184);
        buttonBox = new QDialogButtonBox(kMeans);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(-100, 140, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget = new QWidget(kMeans);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 10, 251, 80));
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

        ImageLabel = new QLabel(kMeans);
        ImageLabel->setObjectName(QStringLiteral("ImageLabel"));
        ImageLabel->setGeometry(QRect(20, 90, 51, 34));
        ImageInput = new QLineEdit(kMeans);
        ImageInput->setObjectName(QStringLiteral("ImageInput"));
        ImageInput->setGeometry(QRect(70, 100, 161, 22));
        browse = new QPushButton(kMeans);
        browse->setObjectName(QStringLiteral("browse"));
        browse->setGeometry(QRect(240, 100, 31, 21));

        retranslateUi(kMeans);
        QObject::connect(buttonBox, SIGNAL(accepted()), kMeans, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), kMeans, SLOT(reject()));

        QMetaObject::connectSlotsByName(kMeans);
    } // setupUi

    void retranslateUi(QDialog *kMeans)
    {
        kMeans->setWindowTitle(QApplication::translate("kMeans", "Dialog", 0));
        ColorLabel->setText(QApplication::translate("kMeans", "Color Count", 0));
        MinSizeLabel->setText(QApplication::translate("kMeans", "Minimum Region Size (pixels)", 0));
        ColorInput->setText(QApplication::translate("kMeans", "2", 0));
        SizeInput->setText(QApplication::translate("kMeans", "5", 0));
        ImageLabel->setText(QApplication::translate("kMeans", "Image:", 0));
        ImageInput->setText(QString());
        browse->setText(QApplication::translate("kMeans", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class kMeans: public Ui_kMeans {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KMEANSUI_H
