
QT       += core

QT       -= gui

TARGET = stereo_mesh_viewer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
	deps/glad.cpp

CONFIG += c++11

INCLUDEPATH += ../../libs deps/glad/include
LIBS += -lGL -lglfw -ldl


DESTDIR = ../../bin
OBJECTS_DIR = obj

