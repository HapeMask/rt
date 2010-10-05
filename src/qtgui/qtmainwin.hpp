#pragma once

#include <QMainWindow>

#include "scene/scene.hpp"
#include "framebuffer/qtoglframebuffer.hpp"

class rtGUI : public QMainWindow {
    public:
        rtGUI(scene& s, QWidget* parent = 0);

    protected:
        void keyPressEvent(QKeyEvent* event) {
            fb->keyPressEvent(event);
        }

    private:
        scene& scn;
        qtOpenGLFramebuffer* fb;
};

