#include <cmath>
#include <iostream>
#include <fstream>

#ifdef RT_MULTITHREADED
#include <omp.h>
#endif

#include <sys/time.h>
#include <unistd.h>

#include <QApplication>
#include "mathlib/SFMT.h"

#include "framebuffer/qtoglframebuffer.hpp"
#include "qtgui/qtmainwin.hpp"

#include "camera/camera.hpp"
#include "scene/scene.hpp"
#include "tracer/tracer.hpp"
#include "scene/sceneloader.hpp"

using namespace std;

int main(int argc, char* args[]){
    // SFMT
    init_gen_rand(time(NULL));

    scene scn;

    string filename = "../src/scene/test.scn";
    int numThreads = 4;
    int c;

    while( (c = getopt(argc, args, "n:")) != -1 ){
        switch(c){
            case 'n':
                numThreads = atoi(optarg);
                break;
            case '?':
                if(optopt == 'n'){
                    cerr << "-n requires an argument" << endl;
                }else{
                    cerr << "Unknown option: -" << optopt << endl;
                }
                break;
            default:
                return 1;
        }
    }

    if(optind < argc){
        filename = string(args[optind]);
    }

    ifstream in(filename.c_str());
    if(!in.is_open()){
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    omp_set_num_threads(numThreads);

    sceneloader::load(in, scn);
    in.close();
    scn.build();
    QApplication app(argc, args);

    rtGUI win(scn);

    //win.resize(scn.getCamera().width(), scn.getCamera().height());
    win.setWindowTitle("Qt/OpenGL rt GUI");
    win.show();

    return app.exec();
}
