#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T19:56:25
#
#-------------------------------------------------

QT       += core gui

TARGET = tsp-sa
TEMPLATE = app


SOURCES += src/mainwindow.cpp \
    src/main.cpp \
    src/draw.cpp

HEADERS  += src/mainwindow.h \
    src/draw.h

FORMS    += ui/mainwindow.ui \
    ui/draw.ui

OTHER_FILES += test_data/input1wsp4dobre.txt \
    test_data/input1wsp3zle.txt \
    test_data/input1wsp2zle.txt \
    test_data/input1wsp.txt \
    test_data/input1.txt
