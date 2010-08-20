#include "mathlib/SFMT.h"

#include "utility.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"

#include "framebuffer/sdlframebuffer.hpp"
#include "camera/camera.hpp"
#include "scene/scene.hpp"

#include "geometry/primitive.hpp"
#include "geometry/triangle.hpp"
#include "geometry/sphere.hpp"

#include "materials/material.hpp"
#include "materials/bsdf.hpp"

#include "color/color.hpp"

#include "light/light.hpp"

#include "tracer/tracer.hpp"

#include "samplers/samplers.hpp"

#include "scene/sceneloader.hpp"

#ifdef RT_MULTITHREADED
#include <omp.h>
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#include <sys/time.h>
#include <SDL.h>

#include <unistd.h>
using namespace std;

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

    sceneloader::load(in, scn);
	in.close();
	scn.build();

	sdlFramebuffer f(scn, 32);

	//srand(time(NULL));
    // SFMT
    init_gen_rand(time(NULL));

#ifdef RT_MULTITHREADED
    omp_set_num_threads(numThreads);
    cerr << "Rendering on " << numThreads << " threads." << endl;
#endif

	//cerr << scn.L(scn.getCamera().width()/2, scn.getCamera().height()/2) << endl;
	//return 0;

    SDL_EnableKeyRepeat(3,3);

    float linTmScale = 1.f;
    struct timeval start, end;

	SDL_Event e;
    bool paused = false, dirty = false, showUpdates = false;

    /*
     * Main interaction loop.
     */
	while(true){
        if(paused){
            SDL_WaitEvent(&e);
        }else{
            const bool done = f.render();
            if(done){
                paused = true;
                continue;
            }

            dirty = true;

            // Consume any events that we aren't interested in.
            int ret;
            do{
                ret = SDL_PollEvent(&e);
            // Only interested in key events and quit events for now.
            }while(e.type != SDL_KEYDOWN && e.type != SDL_QUIT && ret != 0);
        }

		switch(e.type){
			case SDL_KEYDOWN:
				if(e.key.state == SDL_PRESSED || e.key.state == SDL_KEYDOWN){
                    switch(e.key.keysym.sym){
                        case 'q':
                            if((e.key.keysym.mod & KMOD_CTRL) || (e.key.keysym.mod & KMOD_META)){
                                return 0;
                            }
                            break;
                        case SDLK_ESCAPE:
                            if(!paused){
                                cerr << "Pausing..." << endl;
                                paused = true;
                            }

                            break;
                        case '-':
                            linTmScale -= 0.25f;
                            f.setLinearTonemapScale(linTmScale);
                            break;
                        case '=':
                            linTmScale += 0.25f;
                            f.setLinearTonemapScale(linTmScale);
                            break;
                        case 's':
                            if(dirty){
                                cerr << "Saving..." << endl;
                                dirty = false;
                            }
                            break;
                        case 'p':
                            if(paused){
                                cerr << "Resuming..." << endl;
                            }else{
                                cerr << "Pausing..." << endl;
                            }

                            paused = !paused;
                            break;
                        case 'u':
                            showUpdates = !showUpdates;
                            f.setShowUpdates(showUpdates);
                            break;
                        default:
                            break;
                    }
				}
				break;
			case SDL_QUIT:
				return 0;
				break;
            default:
                break;
		}
	}
	return 0;
}
