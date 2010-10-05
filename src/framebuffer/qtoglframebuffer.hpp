#pragma once

#include <QImage>
#include <QGLWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>

#include <GL/gl.h>

#include "framebuffer.hpp"
#include "scene/scene.hpp"
#include "mathlib/vector.hpp"
//#include "qtgui/qtmainwin.hpp"

class qtOpenGLFramebuffer : public QGLWidget, public framebuffer {
    friend class rtGUI;
    // Only if you use signals/slots.
    //Q_OBJECT

    public:
        qtOpenGLFramebuffer(scene& s, const int bpp=32, QWidget* parent = NULL);
        ~qtOpenGLFramebuffer();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        virtual void render();

		inline virtual const bool readyForDrawing() const {
            return true;
        }

    protected:
        void _render(QPainter& painter);
        void initializeGL();
        void resizeGL(int width, int height);

        void paintEvent(QPaintEvent* event);

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void keyPressEvent(QKeyEvent* event);

        virtual void addSample(const int& x, const int& y, const rgbColor& c);
        virtual void setPixel(const int& x, const int& y, const rgbColor& c);

    private:
        GLuint vbo;
        GLfloat* sceneData;

        float viewRotX, viewRotY, fovy;
        QPoint lastPos;
        vec3 camPos, camForward;

        uint64_t pixelsSampled, iterations;
		bool showUpdates, rendered;

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
};
