######################################################################
# Automatically generated by qmake (2.01a) Tue Oct 5 02:49:58 2010
######################################################################

TEMPLATE = app
CONFIG = qt release stl
CONFIG += thread mmx sse sse2
QT += opengl

TARGET = rt

DEFINES += RT_NO_EXECPTIONS \
    RT_MULTITHREADED \
    HAVE_SSE2 \
    MEXP=19937 \
    RT_USE_QT

QMAKE_CXXFLAGS = -march=native -O3 -mmmx -msse -msse2 -msse3 -mssse3 \
	-msse4 -msse4.1 -msse4.2 -ffast-math -freciprocal-math \
	-mfpmath=sse -mrecip -fomit-frame-pointer -pipe -fexpensive-optimizations \
	-funsafe-loop-optimizations -fno-exceptions -fopenmp -fgcse-sm -fgcse-las \
    -funroll-loops -ftree-vectorize -Wno-inline -std=gnu++0x

QMAKE_LIBS += -lGLEW
QMAKE_LFLAGS += -fopenmp -O3

OBJECTS_DIR = ./obj
DEPENDPATH += ../src
INCLUDEPATH += ../src \
    /usr/X11/include

flex.commands = flex ${QMAKE_FILE_IN} && mv lex.yy.cc ../src//scene/
flex.input = FLEXSOURCES
flex.output = ../src/scene/lex.yy.cc
flex.variable_out = SOURCES
flex.name = flex
QMAKE_EXTRA_COMPILERS += flex

bison.commands = bison ${QMAKE_FILE_IN} \
    && mv *.hh ../src//scene/ \
    && mv sceneparser.tab.h ../src/scene/ \
    && sed 's/sceneparser.tab.c/sceneparser.tab.cpp/g' sceneparser.tab.c > ../src/scene/sceneparser.tab.cpp \
    && rm sceneparser.tab.c
bison.input = BISONSOURCES
bison.variable_out = SOURCES
bison.output = ../src/scene/sceneparser.tab.cpp \
    ../src/scene/sceneparser.tab.h \
    ../src/scene/position.hh \
    ../src/scene/location.hh \
    ../src/scene/stack.hh
bison.name = bison
QMAKE_EXTRA_COMPILERS += bison

bisonheader.commands = @true
bisonheader.input = BISONSOURCES
bisonheader.output = ../src/scene/sceneparser.tab.h
bisonheader.variable_out = HEADERS
bisonheader.name = bison header
bisonheader.depends = ../src/scene/sceneparser.tab.cpp
QMAKE_EXTRA_COMPILERS += bisonheader

FLEXSOURCES = ../src/scene/sceneparser.l
BISONSOURCES = ../src/scene/sceneparser.y
HEADERS += ../src/utility.hpp \
           ../src/acceleration/accelerator.hpp \
           ../src/acceleration/bvh.hpp \
           ../src/acceleration/defaultaccelerator.hpp \
           ../src/acceleration/intersection.hpp \
           ../src/acceleration/octree.hpp \
           ../src/camera/camera.hpp \
           ../src/color/color.hpp \
           ../src/datastructs/arraylist.hpp \
           ../src/datastructs/linkedlist.hpp \
           ../src/framebuffer/framebuffer.hpp \
           ../src/framebuffer/qtoglframebuffer.hpp \
           ../src/framebuffer/renderthread.hpp \
           ../src/geometry/aabb.hpp \
           ../src/geometry/meshtriangle.hpp \
           ../src/geometry/primitive.hpp \
           ../src/geometry/shape.hpp \
           ../src/geometry/sphere.hpp \
           ../src/geometry/triangle.hpp \
           ../src/geometry/trianglemesh.hpp \
           ../src/light/light.hpp \
           ../src/materials/bsdf.hpp \
           ../src/materials/texture.hpp \
           ../src/materials/material.hpp \
           ../src/mathlib/constants.hpp \
           ../src/mathlib/matrix.hpp \
           ../src/mathlib/quaternion.hpp \
           ../src/mathlib/ray.hpp \
           ../src/mathlib/SFMT-alti.h \
           ../src/mathlib/SFMT-params.h \
           ../src/mathlib/SFMT-params11213.h \
           ../src/mathlib/SFMT-params1279.h \
           ../src/mathlib/SFMT-params132049.h \
           ../src/mathlib/SFMT-params19937.h \
           ../src/mathlib/SFMT-params216091.h \
           ../src/mathlib/SFMT-params2281.h \
           ../src/mathlib/SFMT-params4253.h \
           ../src/mathlib/SFMT-params44497.h \
           ../src/mathlib/SFMT-params607.h \
           ../src/mathlib/SFMT-params86243.h \
           ../src/mathlib/SFMT-sse2.h \
           ../src/mathlib/SFMT.h \
           ../src/mathlib/sse.hpp \
           ../src/mathlib/transformation.hpp \
           ../src/mathlib/vector.hpp \
           ../src/mathlib/point.hpp \
           ../src/qtgui/qtmainwin.hpp \
           ../src/samplers/samplers.hpp \
           ../src/scene/objparser.hpp \
           ../src/scene/scanner.hpp \
           ../src/scene/scene.hpp \
           ../src/scene/sceneloader.hpp \
           ../src/tracer/tracer.hpp
SOURCES += ../src/main.cpp \
           ../src/utility.cpp \
           ../src/acceleration/bvh.cpp \
           ../src/acceleration/defaultaccelerator.cpp \
           ../src/acceleration/octree.cpp \
           ../src/camera/camera.cpp \
           ../src/color/color.cpp \
           ../src/framebuffer/qtoglframebuffer.cpp \
           ../src/framebuffer/renderthread.cpp \
           ../src/geometry/aabb.cpp \
           ../src/geometry/meshtriangle.cpp \
           ../src/geometry/shape.cpp \
           ../src/geometry/sphere.cpp \
           ../src/geometry/triangle.cpp \
           ../src/light/arealight.cpp \
           ../src/light/spherelight.cpp \
           ../src/light/pointlight.cpp \
           ../src/materials/aniso.cpp \
           ../src/materials/texture.cpp \
           ../src/materials/beckmann.cpp \
           ../src/materials/microfacet.cpp \
           ../src/materials/blinn.cpp \
           ../src/materials/bsdf.cpp \
           ../src/materials/frosted.cpp \
           ../src/materials/bxdf.cpp \
           ../src/materials/material.cpp \
           ../src/materials/phong.cpp \
           ../src/materials/lambertian.cpp \
           ../src/materials/specular.cpp \
           ../src/materials/substrate.cpp \
           ../src/materials/ward.cpp \
           ../src/mathlib/matrix.cpp \
           ../src/mathlib/SFMT.cpp \
           ../src/mathlib/transformation.cpp \
           ../src/mathlib/vector.cpp \
           ../src/mathlib/point.cpp \
           ../src/qtgui/qtmainwin.cpp \
           ../src/samplers/samplers.cpp \
           ../src/scene/objparser.cpp \
           ../src/scene/scene.cpp \
           ../src/tracer/bidir.cpp \
           ../src/tracer/path.cpp \
           ../src/tracer/tracer.cpp \
           ../src/tracer/whitted.cpp
