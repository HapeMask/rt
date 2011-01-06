#include "renderthread.hpp"
#include "qtoglframebuffer.hpp"
#include <omp.h>

renderThread::renderThread(qtOpenGLFramebuffer* f) : fb(f), shouldExit(false), rendering(false) {}

void renderThread::run() {
    while(!shouldExit) {
        mutex.lock();
        if(!rendering){
            renderCond.wait(&mutex);
        }
        mutex.unlock();

        fb->_render();
        emit rendered();
    }
}

void renderThread::wakeup() {
    mutex.lock();
    rendering = true;
    mutex.unlock();

    renderCond.wakeAll();
}

void renderThread::pause() {
    mutex.lock();
    rendering = false;
    mutex.unlock();
}

void renderThread::shutdown() {
    mutex.lock();
    rendering = false;
    shouldExit = true;
    mutex.unlock();

    renderCond.wakeAll();
}
