#include <QPixmap>
#include <sys/time.h>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <iostream>

#include "qtoglframebuffer.hpp"
#include "renderthread.hpp"

#include "mathlib/constants.hpp"
#include "mathlib/quaternion.hpp"
#include "mathlib/vector.hpp"
#include "scene/objparser.hpp"
#include "geometry/sphere.hpp"

#include "utility.hpp"
#include "mathlib/constants.hpp"

using std::cerr;
using std::endl;

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
    viewRotX(0.f), viewRotY(0.f), fovy(s.getCamera().getFov()),
    lastPos(0.f, 0.f),
    camPos(s.getCamera().getPosition()), camForward(s.getCamera().getPosition() - s.getCamera().getLook()),
    pixelsSampled(0), iterations(0),
    showUpdates(false), showRenderView(false), rendering(false),
    imgBuffer(s.getCamera().width(), s.getCamera().height(), QImage::Format_RGB32),
    paused(false), averageSamplesPerSec(0.f), rthread(this)
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

    buffer = new rgbColor[_width*_height];
    sumOfSquares = new rgbColor[_width*_height];
    samplesPerPixel = new int[_width*_height];

    const float sigma = 0.4f;
    float sum = 0.f;
    for(int i=-2; i<=2; ++i){
        for(int j=-2; j<=2; ++j){
            gkern[i+2][j+2] = evaluate2DGaussian(i, j, sigma);
            sum += gkern[i+2][j+2];
        }
    }

    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            gkern[i][j] /= sum;
        }
    }

    connect(&rthread, SIGNAL(rendered()), this, SLOT(redraw()));
    rthread.start();
}

qtOpenGLFramebuffer::~qtOpenGLFramebuffer() {
    rthread.shutdown();
    rthread.wait(10000);

    if(sceneData) delete[] sceneData;
    if(buffer) delete[] buffer;
    if(sumOfSquares) delete[] sumOfSquares;
    if(samplesPerPixel) delete[] samplesPerPixel;
}

QSize qtOpenGLFramebuffer::minimumSizeHint() const {
    return QSize(_width, _height);
}

QSize qtOpenGLFramebuffer::sizeHint() const {
    return QSize(_width, _height);
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
            showRenderView = false;
			break;
        case Qt::Key_A:
			camPos -= right / 5.f;
            showRenderView = false;
			break;
        case Qt::Key_S:
			camPos -= camForward / 5.f;
            showRenderView = false;
			break;
        case Qt::Key_D:
			camPos += right / 5.f;
            showRenderView = false;
			break;
        case Qt::Key_Space:
			camPos += up / 5.f;
            showRenderView = false;
			break;
        case Qt::Key_Z:
			camPos -= up / 5.f;
            showRenderView = false;
			break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            fovy += 2;
            showRenderView = false;
            break;
        case Qt::Key_Minus:
        case Qt::Key_Underscore:
            fovy -= 2;
            showRenderView = false;
            break;
        case Qt::Key_P:
            paused = !paused;
            break;
        case Qt::Key_Escape:
            paused = true;
            break;
        case Qt::Key_R:
            toggleRendering();
            break;
        case Qt::Key_U:
            showUpdates = !showUpdates;
            break;
    }

    if(!showRenderView && iterations != 0){
        emit iterated(0,0);
    }

    scn.getCamera().setPosition(camPos);
    scn.getCamera().setLook(camPos + normalize(camForward));
    scn.getCamera().setFov(fovy);

    update();
}

void qtOpenGLFramebuffer::mousePressEvent(QMouseEvent* event) {
    lastPos = event->pos();
    setFocus(Qt::MouseFocusReason);
}

void qtOpenGLFramebuffer::mouseMoveEvent(QMouseEvent* event) {
    const int dx = event->x() - lastPos.x();
    const int dy = event->y() - lastPos.y();

    if(dx != 0 || dy != 0){
        if(showRenderView){
            emit iterated(0,0);
            showRenderView = false;
        }

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
    gluPerspective(fovy, _width / _height, 0.5, 100);

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
	gluLookAt(camPos.x, camPos.y, camPos.z,
			camPos.x + camForward.x, camPos.y + camForward.y, camPos.z + camForward.z,
			0, 1, 0);
}

void qtOpenGLFramebuffer::clearBuffers() {
    imgBuffer.fill(0);

    const rgbColor black(0.f);
    for(int i=0; i<_width*_height; ++i){
        sumOfSquares[i] = rgbColor(0.f);
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
    glBegin(GL_LINES);

    float samples[400];
    getLDSamples2D(samples, 200);

    vec3 wo(0,0,1);
    glColor3f(0,1,1);
    glVertex3f(0,0,0);
    glVertex3f(wo.x, wo.y, wo.z);

    wo = worldToBsdf(wo, vec3(0,0,1), vec3(0,1,0), vec3(1,0,0));

    vec3 wi;

    blinn* distrib = new blinn(rgbColor(0.9f,0.9f,0.9f), 100);
    microfacetBrdf f(1.6, 1.0, distrib);

    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(5, 0, 0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0, 5, 0);

    glColor3f(0,0,1);
    glVertex3f(0,0,1);
    glVertex3f(0, 0, 5);

    for(int i=0; i < 20; ++i){
        glColor3f(1,0,0);
        const float u0 = sampleUniform(), u1 = samples[2*i], u2 = samples[2*i+1];
        float pdf;
        bxdfType sampledType;

        f.sampleF(u1, u2, wo, wi,pdf);
        wi = bsdfToWorld(wi, vec3(0,0,1), vec3(0,1,0), vec3(1,0,0));

        glVertex3f(0,0,0);
        glVertex3f(wi.x, wi.y, wi.z);
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

    if(showRenderView){
        painter.drawPixmap(0,0, QPixmap::fromImage(imgBuffer));
    }
    painter.endNativePainting();
}

void qtOpenGLFramebuffer::render() {
}

void qtOpenGLFramebuffer::redraw() {
    //QPainter painter(this);
    //painter.beginNativePainting();
    //painter.endNativePainting();

    disableGLOptions();
    QPainter painter;
    painter.begin(this);
    tonemapAndUpdateScreen(painter);
    emit iterated(iterations, averageSamplesPerSec);
    painter.end();
}

void qtOpenGLFramebuffer::_render() {
    if(!showRenderView){
        clearBuffers();
        showRenderView = true;
    }

    struct timeval start, now;
    gettimeofday(&start, NULL);

#ifdef RT_MULTITHREADED
#pragma omp parallel for schedule(dynamic)
#endif
    // Fill blocks as long as the thread can get new ones.
    for(int i=0; i<(HORIZ_BLOCKS * VERT_BLOCKS); ++i){
        int blockCornerX=0, blockCornerY=0;
        getNextBlock(blockCornerY, blockCornerX);

        // Render the pixels in block.
        for(int y=blockCornerY; y< blockCornerY + blockHeight; ++y){
            for(int x=blockCornerX; x < blockCornerX + blockWidth; ++x){
                /*
                if(x != 192 || y != 192){
                    continue;
                }

                cerr << scn.L(x, y) << endl;
                exit(0);
                */

                const float xOffset = sampleUniform() - 0.5f;
                const float yOffset = sampleUniform() - 0.5f;
                const rgbColor L = scn.L((float)x + xOffset, (float)y + yOffset);

#pragma omp critical
                {
                addSample(x, y, L);
                }
            }
        }
    }

    gettimeofday(&now, NULL);
    const float timeElapsed = now.tv_sec - start.tv_sec +
        ((now.tv_usec - start.tv_usec) / 1e6);

    // Lazy.
    averageSamplesPerSec = ((averageSamplesPerSec*iterations) +
        (float)(_width*_height)/timeElapsed) / (iterations+1.f);

    ++iterations;

    blocksUsed = 0;
}

void qtOpenGLFramebuffer::addSample(const int& x, const int& y, const rgbColor& c){
    /*
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            const size_t offset = (y + 2-i) * _width + x + 2 - j;

            if(offset > 0 && offset < _width*_height){
                const rgbColor C = c * gkern[i][j];
                buffer[offset] += C;

                // Don't use weighted values in the statistics data, only use
                // the actual (center) value.
                if(i == 2 && j == 2){
                    sumOfSquares[offset] += c * c;
                }
            }
        }
    }
    */

    const size_t offset = y * _width + x;
    buffer[offset] += c;
    ++samplesPerPixel[offset];
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
    const rgbColor gammaC = clamp(rgbColor(
            powf(c.red(), gamma),
            powf(c.green(), gamma),
            powf(c.blue(), gamma)));

    imgBuffer.setPixel(x, y, qRgb(gammaC.R(), gammaC.G(), gammaC.B()));
}

void qtOpenGLFramebuffer::tonemapAndUpdateScreen(QPainter& painter){
#ifdef RT_MULTITHREADED
#pragma omp parallel for collapse(2)
#endif
    for(int y = 0; y <_height; y++){
        for(int x = 0; x < _width; x++){
            const int offset = y * _width + x;
            setPixel(
                    x, y,
                    clamp(buffer[offset] / (float)samplesPerPixel[offset]));
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
            const int offset = y * _width + x;
            setPixel(
                    x, y,
                    clamp(buffer[offset] / (float)samplesPerPixel[offset]));
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

void qtOpenGLFramebuffer::saveToImage(const QString& filename) const {
    imgBuffer.save(filename);
}

void qtOpenGLFramebuffer::toggleRendering() {
    rendering = !rendering;

    // Wakes up the render thread so it can go to work.
    if(rendering){
        rthread.wakeup();
    }else {
        rthread.pause();
    }
}
