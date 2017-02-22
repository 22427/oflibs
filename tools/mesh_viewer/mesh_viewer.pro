
QT       += core

QT       -= gui

TARGET = mesh_viewer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
  ../../dev/dep/glad/glad.c
CONFIG += c++11


CONFIG += link_pkgconfig
PKGCONFIG += glfw3
LIBS += -ldl -lGL -lm -lpthread -lX11

INCLUDEPATH += ../../libs
INCLUDEPATH += ../../dev/dep

DESTDIR = ../../bin
OBJECTS_DIR = obj

