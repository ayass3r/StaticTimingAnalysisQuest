#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T18:04:52
#
#-------------------------------------------------

QT       += core

QT       -= gui



TARGET = StaticTimingAnalysis_MyVersion
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += main.cpp \
    Circuit.cpp \
    Edge.cpp \
    gate.cpp \
    LIBPARSER.cpp \
    wire.cpp

HEADERS += \
    Circuit.h \
    Edge.h \
    wire.h \
    gate.h
