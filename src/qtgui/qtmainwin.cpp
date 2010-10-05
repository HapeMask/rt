#include "qtmainwin.hpp"
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

#include "framebuffer/qtoglframebuffer.hpp"

rtGUI::rtGUI(scene& s, QWidget* parent) : QMainWindow(parent),
    scn(s), fb(new qtOpenGLFramebuffer(s))
{
    QAction* quit = new QAction("&Quit", this);
    quit->setShortcut(tr("CTRL+Q"));
    QMenu* file = menuBar()->addMenu("&File");
    file->addAction(quit);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    fb->resize(scn.getCamera().width(), scn.getCamera().height());
    setCentralWidget(fb);
}
