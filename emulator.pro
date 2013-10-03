#-------------------------------------------------
#
# Project created by QtCreator 2013-10-02T16:32:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = emulator
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    turingmachine.cpp

HEADERS  += mainwindow.h \
    turingmachine.h

FORMS    += mainwindow.ui
