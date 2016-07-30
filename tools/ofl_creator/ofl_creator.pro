
QT       += core

QT       -= gui

TARGET = ofl_creator 
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    ../../libs/libs/stru.cpp \


CONFIG += c++11


DESTDIR = ../../bin
OBJECTS_DIR = obj

