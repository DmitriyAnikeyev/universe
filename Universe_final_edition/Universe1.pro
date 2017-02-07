#-------------------------------------------------
#
# Project created by QtCreator 2016-04-24T21:32:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Universe1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    mythread.cpp \
    qgrscene.cpp

HEADERS  += dialog.h \
    classes.h \
    mythread.h \
    qgrscene.h

FORMS    += dialog.ui

QMAKE_CXXFLAGS += -std=c++11
