#include <cmath>
#include <iostream>
#include <fstream>

#ifdef RT_USE_OPENMP
#include <omp.h>
#endif

#include <sys/time.h>
#include <unistd.h>

#ifdef RT_USE_QT
#include <QApplication>
#include <QImage>

#include "qtgui/qtmainwin.hpp"
#endif

#include "camera/camera.hpp"
#include "scene/scene.hpp"
#include "tracer/tracer.hpp"
#include "scene/sceneloader.hpp"

#include "materials/texture.hpp"
#include "utility.hpp"

using std::cerr;
using std::endl;
using std::ifstream;

int main(int argc, char* args[]){
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

#ifdef RT_USE_OPENMP
    omp_set_num_threads(numThreads);
#endif

#ifdef RT_USE_QT
    sceneloader::load(in, scn);
    in.close();
    scn.build();
    QApplication app(argc, args);

    rtGUI win(scn);

    win.setWindowTitle("Qt/OpenGL rt GUI");
    win.show();

    return app.exec();
#else
    return 1;
#endif
}
