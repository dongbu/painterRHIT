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
    comandeditor.cpp

HEADERS  += mainwindow.h \
    comandeditor.h

FORMS    += mainwindow.ui

RESOURCES += \
    myres.qrc