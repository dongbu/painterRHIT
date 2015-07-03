#-------------------------------------------------
#
# Project created by QtCreator 2015-06-05T12:10:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FirstAttempt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    guiloadsave.cpp \
    painter.cpp \
    commandinterpreter.cpp \
    drawonwidget.cpp \
    commandeditor.cpp \
    CytonController.cpp \
    commandviewer.cpp

HEADERS  += mainwindow.h \
    guiloadsave.h \
    painter.h \
    commandinterpreter.h \
    drawonwidget.h \
    commandeditor.h \
    CytonController.h \
    commandviewer.h

FORMS    += mainwindow.ui \
    painter.ui \
    commandeditor.ui \
    commandviewer.ui

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
