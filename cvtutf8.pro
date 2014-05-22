#-------------------------------------------------
#
# Project created by QtCreator 2013-09-11T17:51:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cvtutf8
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lxlsreader -liconv -lm 

SOURCES += main.cpp getFileStringUnicode.cpp XlsHandler.cpp

INCLUDEPATH += 

HEADERS += getFileStringUnicode.h XlsHandler.h

QMAKE_CXXFLAGS += -std=c++0x
