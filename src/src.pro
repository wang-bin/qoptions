#-------------------------------------------------
#
# Project created by QtCreator 2011-07-10T15:21:18
#
#-------------------------------------------------

QT       -= gui

TARGET = QOptions
TEMPLATE = lib
CONFIG += staticlib
DEFINES += BUILD_QOPT_LIB

DESTDIR = ../lib
MOC_DIR = .moc/$$[QT_VERSION]
OBJECTS_DIR = .obj

SOURCES += qoptions.cpp
HEADERS += qoptions.h

OTHER_FILES += \
    TODO.txt
