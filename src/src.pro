#-------------------------------------------------
#
# Project created by QtCreator 2011-07-10T15:21:18
#
#-------------------------------------------------

QT       -= gui

TARGET = QOptions
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../lib
MOC_DIR = .moc/$$[QT_VERSION]
OBJECTS_DIR = .obj

SOURCES += qoptions.cpp

HEADERS += qoptions.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    TODO.txt
