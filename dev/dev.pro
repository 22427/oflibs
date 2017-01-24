TEMPLATE = app
TARGET = dev
CONFIG   += console
CONFIG   -= app_bundle



INCLUDEPATH += deps/glad/include

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
    include/all.h \
    include/ogl_geo.h \
    include/ogl_state.h \
    include/ogl_win.h \
    include/socket.h \
    include/stru.h \
    include/track.h \
    include/vd.h \
    include/vdman.h \
    include/vmath.h \
    include/vrpv.h \
    include/ogl_stereo_compositor.h \
    include/sem_queue.h \
    include/processing_graph.h \
    include/mesh.h \
    include/kd_tree.h \
    include/protocol.h \
    include/tnvb.h

SOURCES += \
    src/ogl_geo.cpp \
    src/ogl_state.cpp \
    src/stru.cpp \
    src/track.cpp \
    src/vd.cpp \
    src/vdman.cpp \
    src/vmath.cpp \
    src/vrpv.cpp \
    src/ogl_stereo_compositor.cpp \
    src/mesh.cpp \
    src/kd_tree.cpp \
    src/tnvb.cpp

CONFIG += c++11


DESTDIR = bin
OBJECTS_DIR = obj
#DEFINES += "OFL_GL_HEADER=<glad/glad.h>"
