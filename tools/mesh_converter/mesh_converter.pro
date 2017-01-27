
QT       += core

QT       -= gui

TARGET = mesh_converter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
     ../../dev/dep/glad/glad.c
LIBS += -lglfw -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -ldl -lGL -lm -lpthread
CONFIG += c++11

INCLUDEPATH += ../../libs
INCLUDEPATH += ../../dev/dep
DESTDIR = ../../bin
OBJECTS_DIR = obj

