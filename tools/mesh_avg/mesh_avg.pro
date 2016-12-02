
QT       += core

QT       -= gui

TARGET = mesh_avg
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
	src/main.cpp \
../../libs/libs/vd.cpp\
../../libs/libs/vmath.cpp\
../../libs/libs/stru.cpp\
../../libs/libs/mesh.cpp


CONFIG += c++11

INCLUDEPATH += ../../oflibs
DESTDIR = ../../bin
OBJECTS_DIR = obj

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
