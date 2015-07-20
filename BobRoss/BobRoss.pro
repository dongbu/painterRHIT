#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T08:34:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BobRoss
TEMPLATE = app


SOURCES += ..\src\PaintingGUI\main.cpp\
    ..\src\PaintingGUI\Sketchpad.cpp \
    ..\src\PaintingGUI\CommandWindow.cpp \
    ..\src\PaintingGUI\EditorWindow.cpp \
    ..\src\PaintingGUI\DrawWindow.cpp \
    ..\src\PaintingGUI\Painter.cpp \
    ..\src\PaintingGUI\pugixml.cpp \
    ..\src\PaintingGUI\shapes.cpp \
    ..\src\PaintingGUI\runLogic.cpp \
    ..\src\PaintingGUI\CVImageWidget.cpp

HEADERS  += \
    ..\include\PaintingGUI\Sketchpad.h \
    ..\include\PaintingGUI\CommandWindow.h \
    ..\include\PaintingGUI\EditorWindow.h \
    ..\include\PaintingGUI\Painter.h \
    ..\include\PaintingGUI\pugixml.hpp \
    ..\include\PaintingGUI\pugiconfig.hpp \
    ..\include\PaintingGUI\runLogic.h \
    ..\include\PaintingGUI\CVImageWidget.h

FORMS    += \
    Sketchpad.ui \
    CommandWindow.ui \
    EditorWindow.ui

RESOURCES += \
    myres.qrc
