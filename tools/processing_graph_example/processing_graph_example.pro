
QT       += core

QT       -= gui

TARGET = processing_graph_example 
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \

CONFIG += c++11

INCLUDEPATH += ../../libs
LIBS += -lGL -lglfw -ldl


DESTDIR = ../../bin
OBJECTS_DIR = obj

