TEMPLATE = aux



first.commands = echo "building libs"
first.depends += stock_shaders oflibs

oflibs.target = oflibs
oflibs.commands = mkdir -p ../oflibs &&  ./make_libs.sh ./libs ../oflibs


stock_shaders.target = stock_shaders
stock_shaders.commands =  mkdir -p ../bin/resources && ./libs/resources/make_shaders.sh ../bin/resources
stock_sjaders.depends += oflibs

QMAKE_EXTRA_TARGETS += first stock_shaders oflibs

INCLUDEPATH += deps/glad/include

DISTFILES += \
    libs/ogl_geo.txt \
    libs/ogl_state.txt \
    libs/track.txt \
    libs/vdman.txt \
    libs/vec_math.txt \
    libs/vrpv.txt

HEADERS += \
    libs/all.h \
    libs/ogl_geo.h \
    libs/ogl_state.h \
    libs/ogl_win.h \
    libs/open_gl_tools.h \
    libs/socket.h \
    libs/stru.h \
    libs/track.h \
    libs/vd.h \
    libs/vdman.h \
    libs/vmath.h \
    libs/vrpv.h \
    libs/vr_tools.h

SOURCES += \
    libs/ogl_geo.cpp \
    libs/ogl_state.cpp \
    libs/stru.cpp \
    libs/track.cpp \
    libs/vd.cpp \
    libs/vdman.cpp \
    libs/vmath.cpp \
    libs/vrpv.cpp

CONFIG += c++11


DESTDIR = bin
OBJECTS_DIR = obj
#DEFINES += "OFL_GL_HEADER=<glad/glad.h>"
