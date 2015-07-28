#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T02:07:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BobRoss
TEMPLATE = app


SOURCES +=\
    ../src/PaintingGUI/CommandWindow.cpp \
    ../src/PaintingGUI/CVImageWidget.cpp \
    ../src/PaintingGUI/CytonRunner.cpp \
    ../src/PaintingGUI/drawwindow.cpp \
    ../src/PaintingGUI/imageparser.cpp \
    ../src/PaintingGUI/kmeansSegment.cpp \
    ../src/PaintingGUI/main.cpp \
    ../src/PaintingGUI/Painter.cpp \
    ../src/PaintingGUI/pugixml.cpp \
    ../src/PaintingGUI/RunLogic.cpp \
    ../src/PaintingGUI/shapes.cpp \
    ../src/PaintingGUI/Sketchpad.cpp \
    ../src/PaintingGUI/webcam.cpp \
    workspacewizard.cpp \
    ../src/PaintingGUI/workspacewizard.cpp

HEADERS  += \
    ../include/PaintingGUI/CommandWindow.h \
    ../include/PaintingGUI/CVImageWidget.h \
    ../include/PaintingGUI/CytonRunner.h \
    ../include/PaintingGUI/kmeansSegment.hpp \
    ../include/PaintingGUI/Painter.h \
    ../include/PaintingGUI/pugiconfig.hpp \
    ../include/PaintingGUI/pugixml.hpp \
    ../include/PaintingGUI/runLogic.h \
    ../include/PaintingGUI/Sketchpad.h \
    workspacewizard.h \
    ../include/PaintingGUI/workspacewizard.h

FORMS    += \
    CommandWindow.ui \
    CytonRunner.ui \
    Sketchpad.ui \
    workspacewizard.ui

RESOURCES += \
    myres.qrc
