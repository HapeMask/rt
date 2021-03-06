cmake_minimum_required(VERSION 2.8)
project(rt)

option(ENABLE_EXCEPTIONS "Enable exception support." ON)
option(USE_OPENMP "Enable OpenMP multi-threading support." ON)
option(USE_QT "Enable interactive Qt GUI." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules/)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
if(USE_OPENMP)
    find_package(OpenMP)
    if (NOT OPENMP_FOUND)
        set(USE_OPENMP OFF)
    endif()
endif()
if(USE_QT)
    find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
    find_package(OpenGL)
    find_package(GLEW)

    if (NOT Qt4_FOUND OR NOT QT_QTOPENGL_FOUND OR NOT OPENGL_FOUND OR NOT GLEW_FOUND)
        set(USE_QT OFF)
    endif()
endif()

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(NUM_PARALLEL_LOOPS 4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS}")
set(LINK_FLAGS "${LINK_FLAGS}")

add_definitions(-O3
    -g3 -Wall -Winline -Wextra -Wshadow
    -std=c++11 -fno-rtti
    -march=native
    -mmmx -msse -msse2 -msse3
    -ftree-vectorize -funroll-loops
    -ffast-math -freciprocal-math
    -Wno-unused-parameter)

if(USE_OPENMP)
    add_definitions(-DRT_USE_OPENMP -fopenmp)
endif()

if(USE_QT)
    add_definitions(-DRT_USE_QT)
endif()

if(ENABLE_EXCEPTIONS)
    add_definitions(-DRT_THROW_EXCEPTIONS)
else()
    add_definitions(-fno-exceptions)
endif()

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

message("")
message("==[Options]==")
message("    Use OpenMP:\t\t"     ${USE_OPENMP})
message("    Use Qt:\t\t"     ${USE_QT})
