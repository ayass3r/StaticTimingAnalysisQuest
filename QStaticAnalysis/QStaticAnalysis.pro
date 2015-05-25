#-------------------------------------------------
#
# Project created by QtCreator 2015-05-25T12:24:55
#
#-------------------------------------------------

QT       += core
CONFIG += c++11
QT       -= gui

TARGET = QStaticAnalysis
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Circuit.cpp \
    Edge.cpp \
    gate.cpp \
    STA.cpp \
    wire.cpp

HEADERS += \
    Circuit.h \
    Edge.h \
    gate.h \
    wire.h
