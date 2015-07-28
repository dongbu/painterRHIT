#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <qstring.h>
#include <QObject>
#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QMouseEvent>
#include <qvBoxLayout>
#include <QActionGroup>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <qdialog.h>
#include <qmessagebox>
#include <qpushbutton.h>
#include <qdesktopwidget.h>
#include <qprocess.h>

#include "ui_Sketchpad.h"
#include "ui_commandwindow.h"
#include "ui_CytonRunner.h"

#include <vector>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <future>
#include <math.h>
#include <cstdarg>
#include <fstream>

#include <control/ecEndEffectorSet.h>
#include <controlCore/ecFrameEndEffector.h>
#include <control/ecManipEndEffectorPlace.h>
#include <foundCommon/ecCoordSysXForm.h>
#include <foundCore/ecApplication.h>
#include <foundCore/ecConstants.h>
#include <foundCore/ecMacros.h>
#include <foundCore/ecTypes.h>
#include <manipulation/ecManipulationActionManager.h>
#include <manipulationDirector/ecManipulationScript.h>
#include <remoteCommand/ecRemoteCommand.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>

#include "pugixml.hpp"

//#include <QtCore/QVariant>
//#include <QtWidgets/QAction>
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QButtonGroup>
//#include <QtWidgets/QFrame>
//#include <QtWidgets/QHeaderView>
//#include <QtWidgets/QLabel>
//#include <QtWidgets/QPushButton>
//#include <QtWidgets/QWidget>
//
//#include <QtCore/QVariant>
//#include <QtWidgets/QAction>
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QButtonGroup>
//#include <QtWidgets/QHeaderView>
//#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QMenu>
//#include <QtWidgets/QMenuBar>
//#include <QtWidgets/QStatusBar>
//#include <QtWidgets/QToolBar>
//#include <QtWidgets/QWidget>
//
//#include <QtCore/QVariant>
//#include <QtWidgets/QAction>
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QButtonGroup>
//#include <QtWidgets/QDialog>
//#include <QtWidgets/QDialogButtonBox>
//#include <QtWidgets/QDoubleSpinBox>
//#include <QtWidgets/QFormLayout>
//#include <QtWidgets/QFrame>
//#include <QtWidgets/QGridLayout>
//#include <QtWidgets/QHeaderView>
//#include <QtWidgets/QLabel>
//#include <QtWidgets/QLineEdit>
//#include <QtWidgets/QWidget>
//
//#include <QtCore/QVariant>
//#include <QtWidgets/QAction>
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QButtonGroup>
//#include <QtWidgets/QHBoxLayout>
//#include <QtWidgets/QHeaderView>
//#include <QtWidgets/QLabel>
//#include <QtWidgets/QListWidget>
//#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QMenu>
//#include <QtWidgets/QMenuBar>
//#include <QtWidgets/QPushButton>
//#include <QtWidgets/QStatusBar>
//#include <QtWidgets/QToolBar>
//#include <QtWidgets/QVBoxLayout>
//#include <QtWidgets/QWidget>

