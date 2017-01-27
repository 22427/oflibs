TEMPLATE = app
TARGET = dev
CONFIG   += console
CONFIG   -= app_bundle



INCLUDEPATH += dep/
SOURCES += dep/glad/glad.c \
    00_main.cpp

DISTFILES += \
    doc/ogl_geo.md \
    doc/ogl_state.md \
    doc/track.md \
    doc/vdman.md \
    doc/vmath.md \
    doc/vrpv.md \
    doc/vd.md \
    doc/ogl_win.md \
    doc/processing_graph.md \

HEADERS += \
	ogl_geo.h \
	ogl_state.h \
	ogl_win.h \
	socket.h \
	stru.h \
	track.h \
	vd.h \
	vdman.h \
	vmath.h \
	vrpv.h \
	ogl_stereo_compositor.h \
	sem_queue.h \
	processing_graph.h \
	mesh.h \
	kd_tree.h \
	protocol.h \
	tnvb.h \
	types.h \
	dll.h

SOURCES += \
	ogl_geo.cpp \
	ogl_state.cpp \
	stru.cpp \
	track.cpp \
	vd.cpp \
	vdman.cpp \
	vmath.cpp \
	vrpv.cpp \
	ogl_stereo_compositor.cpp \
	mesh.cpp \
	kd_tree.cpp \
	tnvb.cpp \
	types.cpp

CONFIG += c++11


DESTDIR = bin
OBJECTS_DIR = obj

LIBS += -ldl
#DEFINES += "OFL_GL_HEADER=<glad/glad.h>"
