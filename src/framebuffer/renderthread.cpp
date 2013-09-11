#include "renderthread.hpp"
#include "qtoglframebuffer.hpp"

renderThread::renderThread(qtOpenGLFramebuffer* f) : shouldExit(false), rendering(false), fb(f) {}

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
