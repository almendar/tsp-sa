#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T19:56:25
#
#-------------------------------------------------

QT       += core gui

TARGET = tsp-sa
TEMPLATE = app

CONFIG  += console

SOURCES += main.cpp\
        mainwindow.cpp \
    draw.cpp \
    inputread.cpp \
    tests.cpp \
    tspsolver.cpp

HEADERS  += mainwindow.h \
    draw.h \
    inputread.h \
    tspsolver.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    input1.txt
