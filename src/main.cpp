#include "mathlib/SFMT.h"

#include "camera/camera.hpp"
#include "scene/scene.hpp"
#include "tracer/tracer.hpp"
#include "scene/sceneloader.hpp"

#ifdef RT_MULTITHREADED
#include <omp.h>
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#include <sys/time.h>

#ifdef RT_USE_SDL
#include <SDL.h>
#include "framebuffer/sdlframebuffer.hpp"
#endif

#ifdef RT_USE_QT
#include <QApplication>
#include "framebuffer/qtoglframebuffer.hpp"
#endif

#include <unistd.h>
using namespace std;

int main(int argc, char* args[]){
#ifdef RT_USE_SDL
#include "sdlmain.hpp"
#endif

#ifdef RT_USE_QT
#include "qtmain.hpp"
#endif
}
