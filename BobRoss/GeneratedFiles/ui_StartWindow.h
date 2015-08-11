/********************************************************************************
** Form generated from reading UI file 'StartWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTWINDOW_H
#define UI_STARTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartWindow
{
public:
    QPushButton *newButton;
    QPushButton *loadButton;
    QRadioButton *cameraRadio;
    QRadioButton *sketchRadio;
    QRadioButton *imageRadio;
    QCheckBox *kMeanCheck;
    QCheckBox *cannyCheck;
    QLineEdit *width;
    QLineEdit *height;
    QLabel *X;
    QLabel *aspectRatioLabel;
    QFrame *example;
    QLabel *TitleLabel;
    QLabel *CCLabel;
    QLabel *MRSLabel;
    QLabel *TLabel;
    QLabel *MLLLabel;
    QLineEdit *CCEdit;
    QLineEdit *MRSEdit;
    QLineEdit *TEdit;
    QLineEdit *MLLEdit;

    void setupUi(QWidget *StartWindow)
    {
        if (StartWindow->objectName().isEmpty())
            StartWindow->setObjectName(QStringLiteral("StartWindow"));
        StartWindow->resize(650, 298);
        newButton = new QPushButton(StartWindow);
        newButton->setObjectName(QStringLiteral("newButton"));
        newButton->setGeometry(QRect(560, 260, 75, 23));
        loadButton = new QPushButton(StartWindow);
        loadButton->setObjectName(QStringLiteral("loadButton"));
        loadButton->setGeometry(QRect(460, 260, 75, 23));
        cameraRadio = new QRadioButton(StartWindow);
        cameraRadio->setObjectName(QStringLiteral("cameraRadio"));
        cameraRadio->setGeometry(QRect(240, 30, 82, 17));
        sketchRadio = new QRadioButton(StartWindow);
        sketchRadio->setObjectName(QStringLiteral("sketchRadio"));
        sketchRadio->setGeometry(QRect(240, 70, 82, 17));
        imageRadio = new QRadioButton(StartWindow);
        imageRadio->setObjectName(QStringLiteral("imageRadio"));
        imageRadio->setGeometry(QRect(240, 110, 82, 17));
        kMeanCheck = new QCheckBox(StartWindow);
        kMeanCheck->setObjectName(QStringLiteral("kMeanCheck"));
        kMeanCheck->setGeometry(QRect(330, 110, 70, 17));
        cannyCheck = new QCheckBox(StartWindow);
        cannyCheck->setObjectName(QStringLiteral("cannyCheck"));
        cannyCheck->setGeometry(QRect(330, 170, 70, 17));
        width = new QLineEdit(StartWindow);
        width->setObjectName(QStringLiteral("width"));
        width->setGeometry(QRect(330, 70, 71, 20));
        height = new QLineEdit(StartWindow);
        height->setObjectName(QStringLiteral("height"));
        height->setGeometry(QRect(430, 70, 71, 20));
        X = new QLabel(StartWindow);
        X->setObjectName(QStringLiteral("X"));
        X->setGeometry(QRect(410, 70, 21, 16));
        aspectRatioLabel = new QLabel(StartWindow);
        aspectRatioLabel->setObjectName(QStringLiteral("aspectRatioLabel"));
        aspectRatioLabel->setGeometry(QRect(380, 30, 91, 16));
        example = new QFrame(StartWindow);
        example->setObjectName(QStringLiteral("example"));
        example->setGeometry(QRect(20, 20, 150, 150));
        example->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border: 1px solid;\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(245, 224, 176, 255), stop:0.09 rgba(246, 189, 237, 255), stop:0.14 rgba(194, 207, 246, 255), stop:0.19 rgba(184, 160, 168, 255), stop:0.25 rgba(171, 186, 248, 255), stop:0.32 rgba(243, 248, 224, 255), stop:0.385 rgba(249, 162, 183, 255), stop:0.47 rgba(100, 115, 124, 255), stop:0.58 rgba(251, 205, 202, 255), stop:0.65 rgba(170, 128, 185, 255), stop:0.75 rgba(252, 222, 204, 255), stop:0.805 rgba(206, 122, 218, 255), stop:0.86 rgba(254, 223, 175, 255), stop:0.91 rgba(254, 236, 244, 255), stop:1 rgba(255, 191, 221, 255));"));
        example->setFrameShape(QFrame::StyledPanel);
        example->setFrameShadow(QFrame::Raised);
        TitleLabel = new QLabel(StartWindow);
        TitleLabel->setObjectName(QStringLiteral("TitleLabel"));
        TitleLabel->setGeometry(QRect(240, 10, 71, 16));
        CCLabel = new QLabel(StartWindow);
        CCLabel->setObjectName(QStringLiteral("CCLabel"));
        CCLabel->setGeometry(QRect(420, 120, 91, 16));
        MRSLabel = new QLabel(StartWindow);
        MRSLabel->setObjectName(QStringLiteral("MRSLabel"));
        MRSLabel->setGeometry(QRect(420, 150, 141, 16));
        TLabel = new QLabel(StartWindow);
        TLabel->setObjectName(QStringLiteral("TLabel"));
        TLabel->setGeometry(QRect(420, 180, 81, 16));
        MLLLabel = new QLabel(StartWindow);
        MLLLabel->setObjectName(QStringLiteral("MLLLabel"));
        MLLLabel->setGeometry(QRect(420, 210, 141, 16));
        CCEdit = new QLineEdit(StartWindow);
        CCEdit->setObjectName(QStringLiteral("CCEdit"));
        CCEdit->setGeometry(QRect(580, 120, 51, 20));
        MRSEdit = new QLineEdit(StartWindow);
        MRSEdit->setObjectName(QStringLiteral("MRSEdit"));
        MRSEdit->setGeometry(QRect(580, 150, 51, 20));
        TEdit = new QLineEdit(StartWindow);
        TEdit->setObjectName(QStringLiteral("TEdit"));
        TEdit->setGeometry(QRect(580, 180, 51, 20));
        MLLEdit = new QLineEdit(StartWindow);
        MLLEdit->setObjectName(QStringLiteral("MLLEdit"));
        MLLEdit->setGeometry(QRect(580, 210, 51, 20));

        retranslateUi(StartWindow);

        QMetaObject::connectSlotsByName(StartWindow);
    } // setupUi

    void retranslateUi(QWidget *StartWindow)
    {
        StartWindow->setWindowTitle(QApplication::translate("StartWindow", "Form", 0));
        newButton->setText(QApplication::translate("StartWindow", "Start", 0));
        loadButton->setText(QApplication::translate("StartWindow", "Load", 0));
        cameraRadio->setText(QApplication::translate("StartWindow", "Camera", 0));
        sketchRadio->setText(QApplication::translate("StartWindow", "Sketch", 0));
        imageRadio->setText(QApplication::translate("StartWindow", "Image", 0));
        kMeanCheck->setText(QApplication::translate("StartWindow", "KMean", 0));
        cannyCheck->setText(QApplication::translate("StartWindow", "Canny", 0));
        width->setText(QApplication::translate("StartWindow", "600", 0));
        height->setText(QApplication::translate("StartWindow", "600", 0));
        X->setText(QApplication::translate("StartWindow", "X", 0));
        aspectRatioLabel->setText(QApplication::translate("StartWindow", "Aspect Ratio", 0));
        TitleLabel->setText(QApplication::translate("StartWindow", "Drawing Type", 0));
        CCLabel->setText(QApplication::translate("StartWindow", "Color Count", 0));
        MRSLabel->setText(QApplication::translate("StartWindow", "Minimum Region Size (pixels)", 0));
        TLabel->setText(QApplication::translate("StartWindow", "Threshold (%)", 0));
        MLLLabel->setText(QApplication::translate("StartWindow", "Minimum Line Length (pixels)", 0));
        CCEdit->setText(QApplication::translate("StartWindow", "2", 0));
        MRSEdit->setText(QApplication::translate("StartWindow", "1", 0));
        TEdit->setText(QApplication::translate("StartWindow", "50", 0));
        MLLEdit->setText(QApplication::translate("StartWindow", "5", 0));
    } // retranslateUi

};

namespace Ui {
    class StartWindow: public Ui_StartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWINDOW_H
