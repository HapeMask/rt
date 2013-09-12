#include "qtmainwin.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QtGui>

#include "framebuffer/qtoglframebuffer.hpp"

rtGUI::rtGUI(scene& s, QWidget* _parent) : QMainWindow(_parent),
    scn(s), fb(new qtOpenGLFramebuffer(s))
{
    QAction* ac_quit = new QAction("&Quit", this);
    ac_quit->setShortcut(tr("CTRL+Q"));
    QMenu* file = menuBar()->addMenu("&File");
    file->addAction(ac_quit);
    connect(ac_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(fb, SIGNAL(iterated(uint64_t, float)),
            this, SLOT(iterated(uint64_t, float)));

    QAction* ac_save = new QAction("&Save", this);
    ac_save->setShortcut(tr("CTRL+S"));
    file->addAction(ac_save);
    connect(ac_save, SIGNAL(triggered()), this, SLOT(save()));

    fb->resize(scn.getCamera().width(), scn.getCamera().height());
    QWidget* win = new QWidget(this);
    QHBoxLayout* hbox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout;

    QPushButton* renderButton = new QPushButton("Render", this);
    vbox->addWidget(renderButton);
    connect(renderButton, SIGNAL(clicked()), fb, SLOT(toggleRendering()));

    hbox->addWidget(fb);
    hbox->addLayout(vbox);

    iterLabel = new QLabel("Iterations: 0", win);
    spsLabel = new QLabel("Samples/sec: 0", win);
    statusBar()->addPermanentWidget(iterLabel);
    statusBar()->addPermanentWidget(spsLabel);

    win->setLayout(hbox);
    setCentralWidget(win);
    fb->setFocus(Qt::MouseFocusReason);
}

void rtGUI::iterated(uint64_t iterations, float samplesPerSec) {
    iterLabel->setText(tr("Iterations: ") + QString::number(iterations));
    spsLabel->setText(tr("Samples/sec: ") + QString::number(samplesPerSec));
}

void rtGUI::save() {
    const QString filename =  QFileDialog::getSaveFileName(this, tr("Save Image"), "~", tr("Image Files (*.png *.jpg *.bmp"));
    fb->saveToImage(filename);
}
