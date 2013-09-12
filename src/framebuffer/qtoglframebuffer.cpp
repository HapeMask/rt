#include <QPixmap>
#include <sys/time.h>
#include <unistd.h>

#include <GL/glew.h>

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

qtOpenGLFramebuffer::qtOpenGLFramebuffer(scene& s, const int bpp, QWidget* _parent) :
    QGLWidget(QGLFormat(QGL::DepthBuffer | QGL::DoubleBuffer), _parent),
    framebuffer(s, bpp), vertexVbo(0), normalVbo(0),
    viewRotX(0.f), viewRotY(0.f), fovy(s.getCamera().getFov()),
    lastPos(0.f, 0.f),
    camPos(s.getCamera().getPosition()), camForward(s.getCamera().getPosition() - s.getCamera().getLook()),
    pixelsSampled(0), iterations(0),
    averageSamplesPerSec(0.f),
    showUpdates(false), showRenderView(false), rendering(false),
    imgBuffer(s.getCamera().width(), s.getCamera().height(), QImage::Format_RGB32), rthread(this)
{
    setAutoFillBackground(false);

    makeCurrent();
    GLenum ret = glewInit();
    if(ret != GLEW_OK) {
        rt_throw("Error initializing glew: " + string((const char*)glewGetErrorString(ret)));
    }else{
        cerr << "Using GLEW version: " << glewGetString(GLEW_VERSION) << endl;
    }

    // 3 floats per vertex and 3 floats per vertex normal.
    GLfloat* vertexData = new GLfloat[3 * scn.vertexCount()];
    GLfloat* normalData = new GLfloat[3 * scn.vertexCount()];

    // Dump all triangle vertices and normals into the vbo.
    scn.dumpToVbo(vertexData, normalData);

    // Make a buffer for vertices and normals, then copy the data into them.
    glGenBuffers(1, &vertexVbo);
    glGenBuffers(1, &normalVbo);

    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            scn.vertexCount() * (3 * sizeof(GLfloat)),
            vertexData,
            GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            scn.vertexCount() * (3 * sizeof(GLfloat)),
            normalData,
            GL_STATIC_DRAW);

    // The arrays can be deleted after use, the data is now on the graphics
    // card.
    delete[] vertexData;
    delete[] normalData;

    buffer = new rgbColor[fb_width*fb_height];
    sumOfSquares = new rgbColor[fb_width*fb_height];
    samplesPerPixel = new int[fb_width*fb_height];

    static const float sigma = 0.45f;
    float sum = 0.f;
    for(int i=-3; i<=3; ++i){
        for(int j=-3; j<=3; ++j){
            gkern[i+3][j+3] = evaluate2DGaussian(i, j, sigma);
            sum += gkern[i+3][j+3];
        }
    }

    for(int i=0; i<7; ++i){
        for(int j=0; j<7; ++j){
            gkern[i][j] /= sum;
        }
    }

    connect(&rthread, SIGNAL(rendered()), this, SLOT(redraw()));
    rthread.start();
}

qtOpenGLFramebuffer::~qtOpenGLFramebuffer() {
    rthread.shutdown();
    rthread.wait(10000);

    if(buffer) delete[] buffer;
    if(sumOfSquares) delete[] sumOfSquares;
    if(samplesPerPixel) delete[] samplesPerPixel;

    glDeleteBuffers(1, &vertexVbo);
    glDeleteBuffers(1, &normalVbo);
}

QSize qtOpenGLFramebuffer::minimumSizeHint() const {
    return QSize(fb_width, fb_height);
}

QSize qtOpenGLFramebuffer::sizeHint() const {
    return QSize(fb_width, fb_height);
}

void qtOpenGLFramebuffer::initializeGL(){
    enableGLOptions();
}

void qtOpenGLFramebuffer::keyPressEvent(QKeyEvent* evt){
    if(rendering && evt->key() != Qt::Key_R) {
        return;
    }

    // Camera side vector.
    vec3 right = normalize(cross(camForward, vec3(0, 1, 0)));
    vec3 up = normalize(cross(right, camForward));

    switch(evt->key()){
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
        case Qt::Key_Escape:
            if(rendering) {
                toggleRendering();
            }
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

void qtOpenGLFramebuffer::mousePressEvent(QMouseEvent* evt) {
    if(!rendering) {
        lastPos = evt->pos();
    }

    setFocus(Qt::MouseFocusReason);
}

void qtOpenGLFramebuffer::mouseMoveEvent(QMouseEvent* evt) {
    if(rendering) {
        return;
    }

    const int dx = evt->x() - lastPos.x();
    const int dy = evt->y() - lastPos.y();

    if(dx != 0 || dy != 0){
        if(showRenderView){
            emit iterated(0,0);
            showRenderView = false;
        }

        // Derp multiple inheritance...
        viewRotY += sin(TWOPI * (dx / (float)fb_width));
        viewRotX += sin(TWOPI * (dy / (float)fb_height));

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

        lastPos = evt->pos();

        update();
    }
}

void qtOpenGLFramebuffer::positionCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (float)fb_width / fb_height, 0.5, 100);

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
    for(int i=0; i<fb_width*fb_height; ++i){
        sumOfSquares[i] = rgbColor(0.f);
        buffer[i] = black;
        samplesPerPixel[i] = 0;
    }

    pixelsSampled = 0;
    iterations = 0;
}

void qtOpenGLFramebuffer::resizeGL(int _width, int _height) {
    // Don't allow resizing for now.
    resize(scn.getCamera().width(), scn.getCamera().height());
}

void qtOpenGLFramebuffer::paintEvent(QPaintEvent* evt) {
    enableGLOptions();

    positionCamera();
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat0);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);

    glPushMatrix();

    // Set the locations for the normal and vertex arrays.
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glNormalPointer(GL_FLOAT, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, scn.vertexCount());

    const GLenum err = glGetError();
    if(err != GL_NO_ERROR) { rt_throw(gluErrorString(glGetError())); }

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

    scn.drawGL();
    glPopMatrix();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    disableGLOptions();

    QPainter painter(this);
    painter.beginNativePainting();
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

#ifdef RT_USE_OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
    // Fill blocks as long as the thread can get new ones.
    for(int i=0; i<(HORIZ_BLOCKS * VERT_BLOCKS); ++i){
        int blockCornerX=0, blockCornerY=0;
        getNextBlock(blockCornerX, blockCornerY);

        // Render the pixels in block.
        for(int py=blockCornerY; py< blockCornerY + blockHeight; ++py){
            for(int px=blockCornerX; px < blockCornerX + blockWidth; ++px){
                const float xOffset = sampleUniform() - 0.5f;
                const float yOffset = sampleUniform() - 0.5f;
                const rgbColor L = scn.L((float)px + xOffset, (float)py + yOffset);

#ifdef RT_USE_OPENMP
#pragma omp critical
#endif
                {
                addSample(px, py, L);
                }
            }
        }
    }

    gettimeofday(&now, NULL);
    const float timeElapsed = now.tv_sec - start.tv_sec +
        ((now.tv_usec - start.tv_usec) / 1e6);

    // Lazy.
    averageSamplesPerSec = ((averageSamplesPerSec*iterations) +
        (float)(fb_width*fb_height)/timeElapsed) / (iterations+1.f);
    ++iterations;
    if(iterations == 1024) {
        toggleRendering();
        saveToImage("water-test.png");
    }

    blocksUsed = 0;
}

void qtOpenGLFramebuffer::addSample(const int& px, const int& py, const rgbColor& c){
    for(int i=0; i<7; ++i){
        for(int j=0; j<7; ++j){
            const int offset = (py + 3-i) * fb_width + px + 3 - j;

            if(offset > 0 && offset < fb_width*fb_height){
                const rgbColor C = c * gkern[i][j];
                buffer[offset] += C;
            }
        }
    }

    const int offset = py * fb_width + px;
    ++samplesPerPixel[offset];
    ++pixelsSampled;

    // Don't use weighted values in the statistics data, only use
    // the actual (center) value.
    sumOfSquares[offset] += c * c;
}

void qtOpenGLFramebuffer::enableGLOptions() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
}

void qtOpenGLFramebuffer::disableGLOptions() {
    glDisable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void qtOpenGLFramebuffer::setPixel(const int& px, const int& py, const rgbColor& c) {
    imgBuffer.setPixel(px, py, qRgb(c.R(), c.G(), c.B()));
}

void qtOpenGLFramebuffer::tonemapAndUpdateScreen(QPainter& painter){
#ifdef RT_USE_OPENMP
#pragma omp parallel for collapse(2)
#endif
    for(int py = 0; py <fb_height; py++){
        for(int px = 0; px < fb_width; px++){
            const int offset = py * fb_width + px;

            static const float gamma = 1.f/2.2f;
            const rgbColor& c = buffer[offset] /
                (float)samplesPerPixel[offset];

            const rgbColor gammaC = clamp(rgbColor(
                    pow(c.red(), gamma),
                    pow(c.green(), gamma),
                    pow(c.blue(), gamma)));

            setPixel(px, py, gammaC);
        }
    }

    painter.drawPixmap(0,0, QPixmap::fromImage(imgBuffer));
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
