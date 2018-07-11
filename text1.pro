#-------------------------------------------------
#
# Project created by QtCreator 2017-03-04T18:59:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = text1
TEMPLATE = app


SOURCES += main.cpp\
        Mywidget.cpp \
    Stone.cpp \
    Board.cpp \
    Singlegame.cpp \
    Step.cpp \
    Eveluation.cpp

HEADERS  += Mywidget.h \
    Stone.h \
    Board.h \
    Singlegame.h \
    Step.h \
    Eveluation.h

FORMS    += Mywidget.ui
CONFIG += C++11
