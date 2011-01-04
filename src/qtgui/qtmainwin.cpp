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

rtGUI::rtGUI(scene& s, QWidget* parent) : QMainWindow(parent),
    scn(s), fb(new qtOpenGLFramebuffer(s))
{
    QAction* quit = new QAction("&Quit", this);
    quit->setShortcut(tr("CTRL+Q"));
    QMenu* file = menuBar()->addMenu("&File");
    file->addAction(quit);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(fb, SIGNAL(iterated(uint64_t, float)),
            this, SLOT(iterated(uint64_t, float)));

    QAction* save = new QAction("&Save", this);
    save->setShortcut(tr("CTRL+S"));
    file->addAction(save);
    connect(save, SIGNAL(triggered()), this, SLOT(save()));

    fb->resize(scn.getCamera().width(), scn.getCamera().height());
    QWidget* win = new QWidget(this);
    QHBoxLayout* hbox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout;
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
