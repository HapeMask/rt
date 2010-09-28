#ifdef RT_USE_QT
#include "qtoglframebuffer.hpp"

#include "mathlib/constants.hpp"
#include "mathlib/quaternion.hpp"
#include "mathlib/vector.hpp"
#include "scene/objparser.hpp"

#include "utility.hpp"
#include "mathlib/constants.hpp"

#include <QPixmap>
#include <sys/time.h>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
using namespace std;

GLfloat diffuse0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat ambient0[] = {0.2, 0.2, 0.2, 1.0};
GLfloat specular0[] = {1.0, 1.0, 1.0, 1.0};

// Light Properties
GLfloat light0Pos[] = {-3.0, 5.0, 0.0, 1.0};

// Material Properties
GLfloat diffuseMat0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat ambientMat0[] = {0.3, 0.3, 0.3, 1.0};
GLfloat specularMat0[] = {0.5, 0.5, 0.5, 1.0};

GLfloat shine = 100;

qtOpenGLFramebuffer::qtOpenGLFramebuffer(scene& s, const int bpp, QWidget* parent) :
    QGLWidget(QGLFormat(QGL::DepthBuffer | QGL::DoubleBuffer), parent),
    framebuffer(s, bpp), vbo(0), sceneData(NULL),
    viewRotX(0.f), viewRotY(0.f), fovy(s.getCamera().getFov()), lastPos(0.f, 0.f),
    camPos(s.getCamera().getPosition()), camForward(s.getCamera().getLook()),
    pixelsSampled(0), iterations(0), showUpdates(false), rendered(false),
    imgBuffer(s.getCamera().width(), s.getCamera().height(), QImage::Format_RGB32),
    paused(false)
{
    setAutoFillBackground(false);

    // 3 floats per vertex and 3 floats per vertex normal.
    sceneData = new GLfloat[2 * 3 * s.vertexCount()];
    s.dumpToVbo(sceneData);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            2 * 3 * s.vertexCount() * sizeof(GLfloat),
            sceneData,
            GL_STATIC_DRAW);

    buffer = new rgbColor[width_*height_];
    sumOfSquares = new rgbColor[width_*height_];
    samplesPerPixel = new int[width_*height_];
}

qtOpenGLFramebuffer::~qtOpenGLFramebuffer()
{
    if(sceneData) delete[] sceneData;
}

QSize qtOpenGLFramebuffer::minimumSizeHint() const {
    return QSize(width_, height_);
}

QSize qtOpenGLFramebuffer::sizeHint() const {
    return QSize(width_, height_);
}

void qtOpenGLFramebuffer::initializeGL(){
    glEnable(GL_MULTISAMPLE);
    enableGLOptions();
}

void qtOpenGLFramebuffer::keyPressEvent(QKeyEvent* event){
	// Camera side vector.
	vec3 right = normalize(cross(camForward, vec3(0, 1, 0)));
    vec3 up = normalize(cross(right, camForward));

    switch(event->key()){
        case Qt::Key_W:
			camPos += camForward / 5.f;
            rendered = false;
			break;
        case Qt::Key_A:
			camPos -= right / 5.f;
            rendered = false;
			break;
        case Qt::Key_S:
			camPos -= camForward / 5.f;
            rendered = false;
			break;
        case Qt::Key_D:
			camPos += right / 5.f;
            rendered = false;
			break;
        case Qt::Key_Space:
			camPos += up / 5.f;
            rendered = false;
			break;
        case Qt::Key_Z:
			camPos -= up / 5.f;
            rendered = false;
			break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            fovy += 2;
            rendered = false;
            break;
        case Qt::Key_Minus:
        case Qt::Key_Underscore:
            fovy -= 2;
            rendered = false;
            break;
        case Qt::Key_P:
            paused = !paused;
            break;
        case Qt::Key_Escape:
            paused = true;
            break;
        case Qt::Key_R:
            {
            //QPainter painter(this);
            //painter.beginNativePainting();
            disableGLOptions();
            QPainter painter;
            painter.begin(this);
            _render(painter);
            painter.end();
            //painter.endNativePainting();
            break;
            }
        case Qt::Key_U:
            showUpdates = !showUpdates;
            break;
    }

    scn.getCamera().setPosition(camPos);
    scn.getCamera().setLook(camPos + normalize(camForward));
    scn.getCamera().setFov(fovy);

    update();
}

void qtOpenGLFramebuffer::mousePressEvent(QMouseEvent* event) {
    lastPos = event->pos();
}

void qtOpenGLFramebuffer::mouseMoveEvent(QMouseEvent* event) {
    const int dx = event->x() - lastPos.x();
    const int dy = event->y() - lastPos.y();

    if(dx != 0 || dy != 0){
        rendered = false;

        // Derp multiple inheritance...
        viewRotY += sin(TWOPI * (dx / (float)framebuffer::width()));
        viewRotX += sin(TWOPI * (dy / (float)framebuffer::height()));

        if(viewRotY >= TWOPI){
            viewRotY = 0;
        }else if(viewRotY < 0){
            viewRotY = TWOPI;
        }

        if(viewRotX >= HALFPI){
            viewRotX = HALFPI - EPSILON;
        }else if(viewRotX <= -HALFPI){
            viewRotX = -HALFPI + EPSILON;
        }

        lastPos = event->pos();

        update();
    }
}

void qtOpenGLFramebuffer::positionCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(fovy, width_ / height_, 0.5, 100);

	// Construct quaternions for composite rotation around
	// Y and X axes.
	const quaternion q(cos(-viewRotY/2.0f), vec3(0, 1, 0) * sin(-viewRotY/2.0f));
	const quaternion r(cos(-viewRotX/2.0f), vec3(1, 0, 0) * sin(-viewRotX/2.0f));

	// Canonical view vector (quaternion form).
	const quaternion p(0, 0, 0, -1);

	// Rotate p around Y, then around X, then scale by distance.
	const quaternion pr = qmult(qmult(qmult(q, r), p), qmult(q, r).inverse());
	camForward = normalize(vec3(pr.x, pr.y, pr.z));

    // Update the tracer's camera.
    scn.getCamera().setLook(camPos + normalize(camForward));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos.x(), camPos.y(), camPos.z(),
			camPos.x() + camForward.x(), camPos.y() + camForward.y(), camPos.z() + camForward.z(),
			0, 1, 0);
}

void qtOpenGLFramebuffer::clearBuffers() {
    imgBuffer.fill(0);

    const rgbColor black(0.f);
    for(int i=0; i<width_*height_; ++i){
        buffer[i] = black;
        samplesPerPixel[i] = 0;
    }

    pixelsSampled = 0;
    iterations = 0;
}

void qtOpenGLFramebuffer::resizeGL(int width, int height) {
    // Don't allow resizing for now.
    resize(scn.getCamera().width(), scn.getCamera().height());
}

void qtOpenGLFramebuffer::paintEvent(QPaintEvent* event) {
    enableGLOptions();

    positionCamera();
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glInterleavedArrays(GL_N3F_V3F, 0, sceneData);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat0);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);

    glPushMatrix();

    glDrawArrays(GL_TRIANGLES, 0, scn.vertexCount());
    scn.drawGL();

    /* SAMPLER DEMO CODE
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glBegin(GL_POINTS);

    float samples[400];
    getLDSamples2D(samples, 200);

    for(int i=0; i<200; ++i){
        glColor3f(1,0,0);
        point3 p;
        sampleRectangle(p, vec3(0,2,0), vec3(2,0,0), point3(-2,0,0), samples[2*i], samples[2*i+1]);
        glVertex3f(p.x(), p.y(), p.z());

        glColor3f(0,0,1);
        uniformSampleRectangle(p, vec3(0,2,0), vec3(2,0,0), point3(2,0,0));
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();
    */

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    disableGLOptions();

    QPainter painter(this);
    painter.beginNativePainting();
    //painter.fillRect(0,0,128,128, QColor(0,255,0,128));
    //_render(painter);

    if(rendered){
        painter.drawPixmap(0,0, QPixmap::fromImage(imgBuffer));
    }
    painter.endNativePainting();
}

void qtOpenGLFramebuffer::render() {
    QPainter painter(this);
    painter.beginNativePainting();
    _render(painter);
    painter.endNativePainting();
}

void qtOpenGLFramebuffer::_render(QPainter& painter) {
    if(!rendered){
        clearBuffers();
    }

    struct timeval start, now;
    gettimeofday(&start, NULL);

#ifdef RT_MULTITHREADED
#pragma omp parallel for schedule(dynamic) shared(painter)
#endif
    // Fill blocks as long as the thread can get new ones.
    for(int i=0; i<(HORIZ_BLOCKS * VERT_BLOCKS); ++i){
        int blockCornerX=0, blockCornerY=0;
        getNextBlock(blockCornerY, blockCornerX);

        if(showUpdates){
#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
            {
                painter.eraseRect(blockCornerX, blockCornerY, width_, height_);
                painter.fillRect(
                        blockCornerX, blockCornerY,
                        blockWidth, blockHeight, Qt::green);
                swapBuffers();
            }
        }

        // Render the pixels in block.
        for(int y=blockCornerY; y< blockCornerY + blockHeight; ++y){
            for(int x=blockCornerX; x < blockCornerX + blockWidth; ++x){
                const float xOffset = sampleUniform() - 0.5f;
                const float yOffset = sampleUniform() - 0.5f;

                addSample(
                        x, y,
                        scn.L((float)x + xOffset, (float)y + yOffset)
                    );
            }
        }

        /*
#pragma omp critical
        {
        tonemapAndUpdateRect(painter, blockCornerX, blockCornerY);
        }
        */
    }

    tonemapAndUpdateScreen(painter);

    gettimeofday(&now, NULL);
    const float timeElapsed = now.tv_sec - start.tv_sec +
        ((now.tv_usec - start.tv_usec) / 1e6);

    ++iterations;
    cerr << "Iterations: " << iterations << ", ";
    cerr << "samples/sec: " << (float)(width_*height_)/timeElapsed << endl;

    blocksUsed = 0;
    rendered = true;
}

void qtOpenGLFramebuffer::addSample(const int& x, const int& y, const rgbColor& c){
    const size_t offset = y * width_ + x;
    buffer[offset] += c;
    sumOfSquares[offset] += c * c;
    ++samplesPerPixel[offset];

#ifdef RT_MULTITHREADED
#pragma omp atomic
#endif
            ++pixelsSampled;
}

void qtOpenGLFramebuffer::enableGLOptions() {
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void qtOpenGLFramebuffer::disableGLOptions() {
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}

void qtOpenGLFramebuffer::setPixel(const int& x, const int& y, const rgbColor& c) {
    const float gamma = 1.f/2.2f;
    const rgbColor gammaC = rgbColor(
            powf(c.red(), gamma),
            powf(c.green(), gamma),
            powf(c.blue(), gamma));

    imgBuffer.setPixel(x, y, gammaC.toUint());
}

void qtOpenGLFramebuffer::tonemapAndUpdateScreen(QPainter& painter){
#ifdef RT_MULTITHREADED
#pragma omp parallel for collapse(2)
#endif
    for(int y = 0; y <height_; y++){
        for(int x = 0; x < width_; x++){
            setPixel(x, y,
                    clamp(buffer[y * framebuffer::width() + x] / (float)samplesPerPixel[y * framebuffer::width() + x]));
        }
    }

    painter.drawPixmap(0,0, QPixmap::fromImage(imgBuffer));
}

void qtOpenGLFramebuffer::tonemapAndUpdateRect(QPainter& painter, const int& cornerX, const int& cornerY){
#ifdef RT_MULTITHREADED
#pragma omp parallel for collapse(2)
#endif
    for(int y = cornerY; y < cornerY + blockHeight; y++){
        for(int x = cornerX; x < cornerX + blockWidth; x++){
            setPixel(x, y,
                    clamp(buffer[y * framebuffer::width() + x] / (float)samplesPerPixel[y * framebuffer::width() + x]));
        }
    }

#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
        {
            painter.drawPixmap(
                    QPoint(cornerX, cornerY),
                    QPixmap::fromImage(imgBuffer),
                    QRect(cornerX, cornerY, blockWidth, blockHeight));
        }
}

#endif
