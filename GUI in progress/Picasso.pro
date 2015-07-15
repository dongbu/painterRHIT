#-------------------------------------------------
#
# Project created by QtCreator 2015-06-05T12:10:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Picasso
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    guiloadsave.cpp \
    painter.cpp \
    commandinterpreter.cpp \
    drawonwidget.cpp \
    Line.cpp \
    CytonController.cpp \
    commandviewer.cpp \
    WebcamHandler.cpp \
    DrawWindow.cpp \
    CVImageWidget.cpp

HEADERS  += mainwindow.h \
    guiloadsave.h \
    painter.h \
    commandinterpreter.h \
    drawonwidget.h \
    Line.h \
    CytonController.h \
    commandviewer.h \
    WebcamHandler.h \
    DrawWindow.h \
    CVImageWidget.h

FORMS    += MainWindow.ui \
    Line.ui \
    commandviewer.ui \

RESOURCES += \
    myres.qrc

DISTFILES +=

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui
