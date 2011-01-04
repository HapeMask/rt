#pragma once

#include <QMainWindow>
#include <QLabel>

#include "scene/scene.hpp"
#include "framebuffer/qtoglframebuffer.hpp"

class rtGUI : public QMainWindow {
    Q_OBJECT

    public:
        rtGUI(scene& s, QWidget* parent = 0);

    private:
        scene& scn;
        qtOpenGLFramebuffer* fb;
        QLabel* iterLabel, * spsLabel;

    public slots:
        void iterated(uint64_t iterations, float samplesPerSec);
        void save();
};
