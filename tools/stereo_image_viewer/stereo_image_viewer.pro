
QT       += core

QT       -= gui

TARGET = stereo_image_viewer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    deps/glad.cpp \
	src/stb_image.cpp

CONFIG += c++11

INCLUDEPATH += ../../oflibs deps/glad/include
LIBS += -lGL -lglfw -ldl


DESTDIR = ../../bin
OBJECTS_DIR = obj

HEADERS += \
    src/stb_image.h

#first.commands =  cp ./resources/left.png ../../bin/ && cp ./resources/right.png ../../bin/

QMAKE_EXTRA_TARGETS += first
