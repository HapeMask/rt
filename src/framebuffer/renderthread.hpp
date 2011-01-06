#pragma once
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

#include <iostream>
using std::cerr;
using std::endl;

class qtOpenGLFramebuffer;
class renderThread : public QThread {
    Q_OBJECT

    public:
        renderThread(qtOpenGLFramebuffer* f);

        void wakeup();
        void run();
        void pause();
        void shutdown();

    private:
        bool shouldExit;
        bool rendering;

        qtOpenGLFramebuffer* fb;
        QWaitCondition renderCond;
        QMutex mutex;

    signals:
        void rendered();
};
