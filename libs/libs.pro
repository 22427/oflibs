TEMPLATE = aux



first.commands = echo "building libs"
first.depends += stock_shaders oflibs

oflibs.target = oflibs
oflibs.commands = mkdir -p ../oflibs &&  ./make_libs.sh ./libs ../oflibs


stock_shaders.target = stock_shaders
stock_shaders.commands =  mkdir -p ../bin/resources && ./libs/resources/make_shaders.sh ../bin/resources
stock_shaders.depends += oflibs

QMAKE_EXTRA_TARGETS += first stock_shaders oflibs

INCLUDEPATH += deps/glad/include

DISTFILES += \
    libs/ogl_geo.md \
    libs/ogl_state.md \
    libs/track.md \
    libs/vdman.md \
    libs/vmath.md \
    libs/vrpv.md \
    libs/vd.md \
    libs/ogl_win.md \
    libs/processing_graph.md

HEADERS += \
    libs/all.h \
    libs/ogl_geo.h \
    libs/ogl_state.h \
    libs/ogl_win.h \
    libs/socket.h \
    libs/stru.h \
    libs/track.h \
    libs/vd.h \
    libs/vdman.h \
    libs/vmath.h \
    libs/vrpv.h \
    libs/ogl_stereo_compositor.h \
    libs/sem_queue.h \
    libs/processing_graph.h \
    libs/mesh.h

SOURCES += \
    libs/ogl_geo.cpp \
    libs/ogl_state.cpp \
    libs/stru.cpp \
    libs/track.cpp \
    libs/vd.cpp \
    libs/vdman.cpp \
    libs/vmath.cpp \
    libs/vrpv.cpp \
    libs/ogl_stereo_compositor.cpp \
    libs/mesh.cpp

CONFIG += c++11


DESTDIR = bin
OBJECTS_DIR = obj
#DEFINES += "OFL_GL_HEADER=<glad/glad.h>"
