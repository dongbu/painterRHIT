#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T02:07:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BobRoss
TEMPLATE = app

CONFIG += stdafx.h
PRECOMPILED_HEADER = stdafx.h
win32-msvc* {
PRECOMPILED_SOURCE = stdafx.cpp
}

SOURCES +=\
    ../src/GUI_Classes/CommandWindow.cpp \
    ../src/GUI_Classes/CytonRunner.cpp \
    ../src/GUI_Classes/main.cpp \
    ../src/GUI_Classes/Painter.cpp \
    ../src/GUI_Classes/RunLogic.cpp \
    ../src/GUI_Classes/Sketchpad.cpp \
    ../src/GUI_Classes/stdafx.cpp \
    ../src/GUI_Classes/workspacewizard.cpp \
    ../src/Libraries/CVImageWidget.cpp \
    ../src/Libraries/drawwindow.cpp \
    ../src/Libraries/imageparser.cpp \
    ../src/Libraries/kmeansSegment.cpp \
    ../src/Libraries/pixeltools.cpp \
    ../src/Libraries/pugixml.cpp \
    ../src/Libraries/regiontopaths.cpp \
    ../src/Libraries/shapes.cpp \
    ../src/Libraries/webcam.cpp

HEADERS  += \
    ../include/GUI_Classes/CommandWindow.h \
    ../include/GUI_Classes/CytonRunner.h \
    ../include/GUI_Classes/Painter.h \
    ../include/GUI_Classes/runLogic.h \
    ../include/GUI_Classes/Sketchpad.h \
    ../include/GUI_Classes/stdafx.h \
    ../include/GUI_Classes/workspacewizard.h \
    ../include/Libraries/CVImageWidget.h \
    ../include/Libraries/kmeansSegment.hpp \
    ../include/Libraries/pugiconfig.hpp \
    ../include/Libraries/pugixml.hpp

FORMS    += \
    CommandWindow.ui \
    CytonRunner.ui \
    Sketchpad.ui \
    workspacewizard.ui

RESOURCES += \
    myres.qrc
