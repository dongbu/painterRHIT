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
    commandeditor.cpp \
    painter.cpp \
    commandinterpreter.cpp \
    drawonwidget.cpp

HEADERS  += mainwindow.h \
    guiloadsave.h \
    commandeditor.h \
    painter.h \
    commandinterpreter.h \
    drawonwidget.h

FORMS    += mainwindow.ui \
    painter.ui

RESOURCES += \
    myres.qrc

DISTFILES +=
