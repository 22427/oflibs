
QT       += core

QT       -= gui

TARGET = mesh_converter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \

CONFIG += c++11

INCLUDEPATH += ../../oflibs
DESTDIR = ../../bin
OBJECTS_DIR = obj

