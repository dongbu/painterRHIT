#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T08:34:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BobRoss
TEMPLATE = app


SOURCES += main.cpp\
    Sketchpad.cpp \
    CommandWindow.cpp \
    EditorWindow.cpp \
    DrawWindow.cpp \
    Painter.cpp \
    pugixml.cpp \
    shapes.cpp \
    CVImageWidget.cpp

HEADERS  += \
    Sketchpad.h \
    CommandWindow.h \
    EditorWindow.h \
    Painter.h \
    pugixml.hpp \
    CVImageWidget.h

FORMS    += \
    Sketchpad.ui \
    CommandWindow.ui \
    EditorWindow.ui

RESOURCES += \
    myres.qrc
