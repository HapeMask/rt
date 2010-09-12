#ifdef RT_USE_QT
#include "qtoglframebuffer.hpp"

#include "mathlib/constants.hpp"
#include "mathlib/quaternion.hpp"
#include "mathlib/vector.hpp"
#include "scene/objparser.hpp"

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

qtOpenGLFramebuffer::qtOpenGLFramebuffer(const scene& s, const int bpp, QWidget* parent) :
    QGLWidget(QGLFormat(QGL::DepthBuffer | QGL::DoubleBuffer), parent),
    framebuffer(s.getCamera().width(), s.getCamera().height(), bpp),
    vbo(0), sceneData(NULL),
    viewRotX(0.f), viewRotY(0.f), lastPos(0.f, 0.f),
    camPos(s.getCamera().getPosition()), camForward(s.getCamera().getLook()), scn(s)
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
}

qtOpenGLFramebuffer::~qtOpenGLFramebuffer()
{
    if(sceneData) delete[] sceneData;
}

QSize qtOpenGLFramebuffer::minimumSizeHint() const {
    return QSize(400, 400);
}

QSize qtOpenGLFramebuffer::sizeHint() const {
    return QSize(400, 400);
}

void qtOpenGLFramebuffer::initializeGL(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void qtOpenGLFramebuffer::keyPressEvent(QKeyEvent* event){
	// Camera side vector.
	vec3 right = normalize(cross(camForward, vec3(0, 1, 0)));
    vec3 up = normalize(cross(right, camForward));

    switch(event->key()){
        case Qt::Key_W:
			camPos += camForward / 5.f;
			break;
        case Qt::Key_A:
			camPos -= right / 5.f;
			break;
        case Qt::Key_S:
			camPos -= camForward / 5.f;
			break;
        case Qt::Key_D:
			camPos += right / 5.f;
			break;
        case Qt::Key_Space:
			camPos += up / 5.f;
			break;
        case Qt::Key_Z:
			camPos -= up / 5.f;
			break;
    }

    update();
}

void qtOpenGLFramebuffer::mousePressEvent(QMouseEvent* event) {
    lastPos = event->pos();
}

void qtOpenGLFramebuffer::mouseMoveEvent(QMouseEvent* event) {
    const int dx = event->x() - lastPos.x();
    const int dy = event->y() - lastPos.y();

    if(dx != 0 || dy != 0){
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
    gluPerspective(45, 1.0, 0.5, 100);
    glTranslatef(0,0,-2);

	// Construct quaternions for composite rotation around
	// Y and X axes.
	const quaternion q(cos(-viewRotY/2.0f), vec3(0, 1, 0) * sin(-viewRotY/2.0f));
	const quaternion r(cos(-viewRotX/2.0f), vec3(1, 0, 0) * sin(-viewRotX/2.0f));

	// Canonical view vector (quaternion form).
	const quaternion p(0, 0, 0, -1);

	// Rotate p around Y, then around X, then scale by distance.
	const quaternion pr = qmult(qmult(qmult(q, r), p), qmult(q, r).inverse());
	camForward = normalize(vec3(pr.x, pr.y, pr.z));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos.x(), camPos.y(), camPos.z(),
			camPos.x() + camForward.x(), camPos.y() + camForward.y(), camPos.z() + camForward.z(),
			0, 1, 0);
}

void qtOpenGLFramebuffer::resizeGL(int width, int height) {
    // Don't allow resizing for now.
    resize(scn.getCamera().width(), scn.getCamera().height());
}

void qtOpenGLFramebuffer::paintEvent(QPaintEvent* event) {
    initializeGL();

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

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_NORMALIZE);

    QPainter painter(this);
    painter.beginNativePainting();
    painter.fillRect(0,0,128,128, QColor(0,255,0,128));
    painter.endNativePainting();
}

const bool qtOpenGLFramebuffer::render() {
}
#endif
