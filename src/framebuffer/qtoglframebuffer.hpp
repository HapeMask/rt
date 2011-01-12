#pragma once

#include <GL/glew.h>

#include <QImage>
#include <QGLWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>

#include "framebuffer.hpp"
#include "scene/scene.hpp"
#include "mathlib/vector.hpp"

#include "renderthread.hpp"
class qtOpenGLFramebuffer : public QGLWidget, public framebuffer {
    // Only if you use signals/slots.
    Q_OBJECT

    friend class renderThread;

    public:
        qtOpenGLFramebuffer(scene& s, const int bpp=32, QWidget* parent = NULL);
        virtual ~qtOpenGLFramebuffer();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        virtual void render();

        inline const uint64_t& getIterations() const {
            return iterations;
        }

        void saveToImage(const QString& filename) const;

    protected:
        void _render();
        void initializeGL();
        void resizeGL(int width, int height);

        void paintEvent(QPaintEvent* event);

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void keyPressEvent(QKeyEvent* event);

        virtual void addSample(const int& x, const int& y, const rgbColor& c);
        virtual void setPixel(const int& x, const int& y, const rgbColor& c);

    private:
        GLuint vertexVbo, normalVbo;

        float viewRotX, viewRotY, fovy;
        QPoint lastPos;
        point3 camPos;
        vec3 camForward;

        uint64_t pixelsSampled, iterations;
        float averageSamplesPerSec;
		bool showUpdates, showRenderView, rendering;

        rgbColor* buffer;
        rgbColor* sumOfSquares;
        int* samplesPerPixel;
        QImage imgBuffer;
        bool paused;

        void tonemapAndUpdateScreen(QPainter& painter);
        void tonemapAndUpdateRect(QPainter& painter, const int& cornerX, const int& cornerY);

        void positionCamera();
        void enableGLOptions();
        void disableGLOptions();

        void clearBuffers();

        float gkern[5][5];

        renderThread rthread;

    signals:
        void iterated(uint64_t iterations, float samplesPerSec);

    private slots:
        void redraw();
        void toggleRendering();
};
