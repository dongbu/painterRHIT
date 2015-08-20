/********************************************************************************
** Form generated from reading UI file 'Sketchpad.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SKETCHPAD_H
#define UI_SKETCHPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sketchpad
{
public:
    QAction *actionOpen;
    QAction *actionNew;
    QAction *actionDraw_Square;
    QAction *actionDraw_Circle;
    QAction *actionDraw_Line;
    QAction *actionSave_As;
    QAction *actionSave;
    QAction *actionDraw_Filled_Circle;
    QAction *actionDraw_Filled_Rectangle;
    QAction *actionDraw_Filled_Polygon;
    QAction *actionShutdown;
    QAction *actionLoad;
    QAction *actionCreate;
    QAction *actionActionFill;
    QAction *actionCalibrate;
    QAction *actionLoad_Photo_Canny;
    QAction *actionLoad_Photo_Kmeans;
    QAction *actionView;
    QAction *actionSwitch;
    QAction *actionJudge;
    QAction *actionSnap_Webcam_Picture;
    QAction *actionClear;
    QAction *actionCyton;
    QAction *actionABB;
    QAction *actionConnect;
    QAction *actionSet_sketch_window_size;
    QAction *actionLoadPhoto;
    QAction *actionDefine_Shape;
    QWidget *centralwidget;
    QWidget *widget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuRobot;
    QMenu *menuWorkspace;
    QMenu *menuSelect_Robot;
    QMenu *menuWebcam;
    QMenu *menuImage;
    QMenu *menuBrush;
    QStatusBar *statusbar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *Sketchpad)
    {
        if (Sketchpad->objectName().isEmpty())
            Sketchpad->setObjectName(QStringLiteral("Sketchpad"));
        Sketchpad->resize(924, 857);
        actionOpen = new QAction(Sketchpad);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon Storage/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionNew = new QAction(Sketchpad);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon Storage/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon1);
        actionDraw_Square = new QAction(Sketchpad);
        actionDraw_Square->setObjectName(QStringLiteral("actionDraw_Square"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Icon Storage/Square.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Square->setIcon(icon2);
        actionDraw_Circle = new QAction(Sketchpad);
        actionDraw_Circle->setObjectName(QStringLiteral("actionDraw_Circle"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Icon Storage/Circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Circle->setIcon(icon3);
        actionDraw_Line = new QAction(Sketchpad);
        actionDraw_Line->setObjectName(QStringLiteral("actionDraw_Line"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Icon Storage/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Line->setIcon(icon4);
        actionSave_As = new QAction(Sketchpad);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Icon Storage/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon5);
        actionSave = new QAction(Sketchpad);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave->setIcon(icon5);
        actionDraw_Filled_Circle = new QAction(Sketchpad);
        actionDraw_Filled_Circle->setObjectName(QStringLiteral("actionDraw_Filled_Circle"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Icon Storage/FilledCircle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Filled_Circle->setIcon(icon6);
        actionDraw_Filled_Rectangle = new QAction(Sketchpad);
        actionDraw_Filled_Rectangle->setObjectName(QStringLiteral("actionDraw_Filled_Rectangle"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Icon Storage/FilledSquare.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Filled_Rectangle->setIcon(icon7);
        actionDraw_Filled_Polygon = new QAction(Sketchpad);
        actionDraw_Filled_Polygon->setObjectName(QStringLiteral("actionDraw_Filled_Polygon"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/Icon Storage/FreeForm.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDraw_Filled_Polygon->setIcon(icon8);
        actionShutdown = new QAction(Sketchpad);
        actionShutdown->setObjectName(QStringLiteral("actionShutdown"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/Icon Storage/OffButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShutdown->setIcon(icon9);
        actionLoad = new QAction(Sketchpad);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionLoad->setIcon(icon);
        actionCreate = new QAction(Sketchpad);
        actionCreate->setObjectName(QStringLiteral("actionCreate"));
        actionCreate->setIcon(icon1);
        actionActionFill = new QAction(Sketchpad);
        actionActionFill->setObjectName(QStringLiteral("actionActionFill"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/Icon Storage/fill.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionActionFill->setIcon(icon10);
        actionCalibrate = new QAction(Sketchpad);
        actionCalibrate->setObjectName(QStringLiteral("actionCalibrate"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/Icon Storage/wrench.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCalibrate->setIcon(icon11);
        actionLoad_Photo_Canny = new QAction(Sketchpad);
        actionLoad_Photo_Canny->setObjectName(QStringLiteral("actionLoad_Photo_Canny"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/Icon Storage/trace.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Photo_Canny->setIcon(icon12);
        actionLoad_Photo_Kmeans = new QAction(Sketchpad);
        actionLoad_Photo_Kmeans->setObjectName(QStringLiteral("actionLoad_Photo_Kmeans"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/Icon Storage/splatter.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Photo_Kmeans->setIcon(icon13);
        actionView = new QAction(Sketchpad);
        actionView->setObjectName(QStringLiteral("actionView"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/Icon Storage/eyeball.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionView->setIcon(icon14);
        actionSwitch = new QAction(Sketchpad);
        actionSwitch->setObjectName(QStringLiteral("actionSwitch"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/Icon Storage/switch.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSwitch->setIcon(icon15);
        actionJudge = new QAction(Sketchpad);
        actionJudge->setObjectName(QStringLiteral("actionJudge"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/Icon Storage/gavel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionJudge->setIcon(icon16);
        actionSnap_Webcam_Picture = new QAction(Sketchpad);
        actionSnap_Webcam_Picture->setObjectName(QStringLiteral("actionSnap_Webcam_Picture"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/Icon Storage/webcam.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSnap_Webcam_Picture->setIcon(icon17);
        actionClear = new QAction(Sketchpad);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/Icon Storage/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon18);
        actionCyton = new QAction(Sketchpad);
        actionCyton->setObjectName(QStringLiteral("actionCyton"));
        actionABB = new QAction(Sketchpad);
        actionABB->setObjectName(QStringLiteral("actionABB"));
        actionConnect = new QAction(Sketchpad);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        actionConnect->setIcon(icon9);
        actionSet_sketch_window_size = new QAction(Sketchpad);
        actionSet_sketch_window_size->setObjectName(QStringLiteral("actionSet_sketch_window_size"));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/Icon Storage/resize.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSet_sketch_window_size->setIcon(icon19);
        actionLoadPhoto = new QAction(Sketchpad);
        actionLoadPhoto->setObjectName(QStringLiteral("actionLoadPhoto"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/Icon Storage/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadPhoto->setIcon(icon20);
        actionDefine_Shape = new QAction(Sketchpad);
        actionDefine_Shape->setObjectName(QStringLiteral("actionDefine_Shape"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/Icon Storage/sizeBrush.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDefine_Shape->setIcon(icon21);
        centralwidget = new QWidget(Sketchpad);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 901, 750));
        widget->setCursor(QCursor(Qt::CrossCursor));
        widget->setStyleSheet(QStringLiteral("border: 1px solid black;"));
        Sketchpad->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Sketchpad);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 924, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRobot = new QMenu(menubar);
        menuRobot->setObjectName(QStringLiteral("menuRobot"));
        menuWorkspace = new QMenu(menuRobot);
        menuWorkspace->setObjectName(QStringLiteral("menuWorkspace"));
        menuSelect_Robot = new QMenu(menuRobot);
        menuSelect_Robot->setObjectName(QStringLiteral("menuSelect_Robot"));
        menuWebcam = new QMenu(menubar);
        menuWebcam->setObjectName(QStringLiteral("menuWebcam"));
        menuImage = new QMenu(menubar);
        menuImage->setObjectName(QStringLiteral("menuImage"));
        menuBrush = new QMenu(menubar);
        menuBrush->setObjectName(QStringLiteral("menuBrush"));
        Sketchpad->setMenuBar(menubar);
        statusbar = new QStatusBar(Sketchpad);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Sketchpad->setStatusBar(statusbar);
        toolBar_2 = new QToolBar(Sketchpad);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        Sketchpad->addToolBar(Qt::TopToolBarArea, toolBar_2);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuRobot->menuAction());
        menubar->addAction(menuWebcam->menuAction());
        menubar->addAction(menuImage->menuAction());
        menubar->addAction(menuBrush->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuRobot->addAction(menuSelect_Robot->menuAction());
        menuRobot->addAction(actionConnect);
        menuRobot->addAction(menuWorkspace->menuAction());
        menuWorkspace->addAction(actionCreate);
        menuWorkspace->addAction(actionLoad);
        menuSelect_Robot->addAction(actionCyton);
        menuSelect_Robot->addAction(actionABB);
        menuWebcam->addAction(actionCalibrate);
        menuWebcam->addAction(actionView);
        menuWebcam->addAction(actionSwitch);
        menuWebcam->addAction(actionJudge);
        menuImage->addAction(actionLoad_Photo_Kmeans);
        menuImage->addAction(actionLoad_Photo_Canny);
        menuImage->addAction(actionSnap_Webcam_Picture);
        menuImage->addAction(actionSet_sketch_window_size);
        menuBrush->addAction(actionDefine_Shape);
        toolBar_2->addAction(actionClear);
        toolBar_2->addSeparator();
        toolBar_2->addAction(actionDraw_Line);
        toolBar_2->addAction(actionDraw_Circle);
        toolBar_2->addAction(actionDraw_Square);
        toolBar_2->addSeparator();
        toolBar_2->addAction(actionActionFill);
        toolBar_2->addAction(actionDraw_Filled_Rectangle);
        toolBar_2->addAction(actionDraw_Filled_Circle);
        toolBar_2->addAction(actionDraw_Filled_Polygon);
        toolBar_2->addSeparator();
        toolBar_2->addAction(actionLoadPhoto);
        toolBar_2->addSeparator();

        retranslateUi(Sketchpad);

        QMetaObject::connectSlotsByName(Sketchpad);
    } // setupUi

    void retranslateUi(QMainWindow *Sketchpad)
    {
        Sketchpad->setWindowTitle(QApplication::translate("Sketchpad", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("Sketchpad", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("Sketchpad", "Ctrl+O", 0));
        actionNew->setText(QApplication::translate("Sketchpad", "New", 0));
        actionNew->setShortcut(QApplication::translate("Sketchpad", "Ctrl+N", 0));
        actionDraw_Square->setText(QApplication::translate("Sketchpad", "Draw Square", 0));
        actionDraw_Circle->setText(QApplication::translate("Sketchpad", "Draw Circle", 0));
        actionDraw_Line->setText(QApplication::translate("Sketchpad", "Draw Line", 0));
        actionSave_As->setText(QApplication::translate("Sketchpad", "Save As", 0));
        actionSave->setText(QApplication::translate("Sketchpad", "Save", 0));
        actionSave->setShortcut(QApplication::translate("Sketchpad", "Ctrl+S", 0));
        actionDraw_Filled_Circle->setText(QApplication::translate("Sketchpad", "Draw Filled Circle", 0));
        actionDraw_Filled_Rectangle->setText(QApplication::translate("Sketchpad", "Draw Filled Rectangle", 0));
        actionDraw_Filled_Polygon->setText(QApplication::translate("Sketchpad", "Draw Filled Polygon", 0));
        actionShutdown->setText(QApplication::translate("Sketchpad", "Shutdown", 0));
        actionLoad->setText(QApplication::translate("Sketchpad", "load", 0));
        actionCreate->setText(QApplication::translate("Sketchpad", "create", 0));
        actionActionFill->setText(QApplication::translate("Sketchpad", "actionFill", 0));
#ifndef QT_NO_TOOLTIP
        actionActionFill->setToolTip(QApplication::translate("Sketchpad", "Fill Region", 0));
#endif // QT_NO_TOOLTIP
        actionCalibrate->setText(QApplication::translate("Sketchpad", "Calibrate", 0));
#ifndef QT_NO_TOOLTIP
        actionCalibrate->setToolTip(QApplication::translate("Sketchpad", "launch webcam", 0));
#endif // QT_NO_TOOLTIP
        actionLoad_Photo_Canny->setText(QApplication::translate("Sketchpad", "Load Photo Canny", 0));
        actionLoad_Photo_Kmeans->setText(QApplication::translate("Sketchpad", "Load Photo Kmeans", 0));
        actionView->setText(QApplication::translate("Sketchpad", "View", 0));
        actionSwitch->setText(QApplication::translate("Sketchpad", "Switch Webcam", 0));
        actionJudge->setText(QApplication::translate("Sketchpad", "Judge", 0));
        actionSnap_Webcam_Picture->setText(QApplication::translate("Sketchpad", "Snap Webcam Picture", 0));
        actionClear->setText(QApplication::translate("Sketchpad", "clear", 0));
#ifndef QT_NO_TOOLTIP
        actionClear->setToolTip(QApplication::translate("Sketchpad", "clear all", 0));
#endif // QT_NO_TOOLTIP
        actionCyton->setText(QApplication::translate("Sketchpad", "Cyton", 0));
        actionABB->setText(QApplication::translate("Sketchpad", "ABB", 0));
        actionConnect->setText(QApplication::translate("Sketchpad", "Connect", 0));
        actionSet_sketch_window_size->setText(QApplication::translate("Sketchpad", "Set sketch window size", 0));
        actionLoadPhoto->setText(QApplication::translate("Sketchpad", "loadPhoto", 0));
#ifndef QT_NO_TOOLTIP
        actionLoadPhoto->setToolTip(QApplication::translate("Sketchpad", "Load Photo", 0));
#endif // QT_NO_TOOLTIP
        actionDefine_Shape->setText(QApplication::translate("Sketchpad", "Define Shape", 0));
#ifndef QT_NO_TOOLTIP
        actionDefine_Shape->setToolTip(QApplication::translate("Sketchpad", "Define Brush Shape", 0));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("Sketchpad", "File", 0));
        menuRobot->setTitle(QApplication::translate("Sketchpad", "Robot", 0));
        menuWorkspace->setTitle(QApplication::translate("Sketchpad", "Canvas", 0));
        menuSelect_Robot->setTitle(QApplication::translate("Sketchpad", "Select Robot", 0));
        menuWebcam->setTitle(QApplication::translate("Sketchpad", "Webcam", 0));
        menuImage->setTitle(QApplication::translate("Sketchpad", "Image", 0));
        menuBrush->setTitle(QApplication::translate("Sketchpad", "Brush", 0));
        toolBar_2->setWindowTitle(QApplication::translate("Sketchpad", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class Sketchpad: public Ui_Sketchpad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKETCHPAD_H
