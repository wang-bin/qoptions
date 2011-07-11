#-------------------------------------------------
#
# Project created by QtCreator 2011-07-10T15:24:56
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ./
INCLUDEPATH += ../src
#LIBS += -L../lib -lQOptons

SOURCES += main.cpp \
	../src/qoptions.cpp

HEADERS += \
	../src/qoptions.h
