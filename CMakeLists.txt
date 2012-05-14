cmake_minimum_required(VERSION 2.8)
project(rt)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules/)
find_package(OpenMP REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(NUM_PARALLEL_LOOPS 4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} -flto")
set(LINK_FLAGS "${LINK_FLAGS} -flto")

add_definitions(-O3
    -g -Wall -std=c++11
    -fno-rtti -fno-exceptions
    -m64 -march=native
    -mmmx -msse -msse2 -mfpmath=sse
    -ftree-vectorize -fopenmp
    -funroll-loops -funsafe-loop-optimizations
    -floop-parallelize-all
    -ftree-parallelize-loops=${NUM_PARALLEL_LOOPS}
    -ftree-loop-distribution -fgraphite-identity
    -ffast-math -freciprocal-math -mrecip
    -pipe -fomit-frame-pointer
    -fexpensive-optimizations
    -fgcse-sm -fgcse-las -flto
    -Wno-narrowing
    -Wno-inline -Wno-unused-parameter)

add_definitions(-DRT_MULTITHREADED
                -DHAVE_SSE2
                -DMEXP=19937
                -DRT_USE_QT
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/scene
    ${CMAKE_BINARY_DIR}/src/scene
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${QT_INCLUDE_DIR})

add_library(Utility STATIC
    ${CMAKE_SOURCE_DIR}/src/utility.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/acceleration)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/camera)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/color)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/framebuffer)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/geometry)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/light)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/materials)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/mathlib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/qtgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/samplers)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/scene)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/tracer)

set(RT_HEADERS
    ${CMAKE_SOURCE_DIR}/src/framebuffer/qtoglframebuffer.hpp
    ${CMAKE_SOURCE_DIR}/src/framebuffer/renderthread.hpp
    ${CMAKE_SOURCE_DIR}/src/qtgui/qtmainwin.hpp)
QT4_WRAP_CPP(RT_HEADERS_MOC ${RT_HEADERS})

add_executable(rt
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${RT_HEADERS_MOC}
    )

target_link_libraries(rt
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    Acceleration
    Camera
    Color
    Framebuffer
    Geometry
    Light
    Materials
    Mathlib
    Samplers
    Scene
    QtGUI
    Utility
    )
