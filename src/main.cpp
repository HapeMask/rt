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
#include "tracer/path.hpp"

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
using namespace std;

// Default to 8 threads.
#ifndef RT_OMP_THREADS
#define RT_OMP_THREADS 8
#endif

void draw(const int height, const int width, const camera& c, sdlFramebuffer& f, const rayTracer& rt, const unsigned int blockSize);

int main(int argc, char* args[]){
	scene s;

    string filename;

	int numThreads = RT_OMP_THREADS;
    if(argc < 2){
        filename = "../src/scene/test.scn";
    }else{
        filename = string(args[1]);
    }

	if(argc == 3){
		numThreads = atoi(args[2]);
	}

	ifstream in(filename.c_str());
    if(!in.is_open()){
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    sceneloader::load(in, s);
	in.close();

	s.build();

	camera& c = *s.getCamera();
    const int width = c.width();
    const int height = c.height();

	sdlFramebuffer f(width, height, 32);

	pathTracer rt(&s);
	//whittedRayTracer rt(&s);

	//srand(time(NULL));
    // SFMT
    init_gen_rand(time(NULL));

    /*
    ray r0 = c.getRay(256,256);
    cerr << rt.L(r0) << endl;
    return 0;
    */

#ifdef RT_MULTITHREADED
    omp_set_num_threads(numThreads);
    cerr << "Rendering on " << numThreads << " threads." << endl;
#endif

	struct timeval start, end;
	gettimeofday(&start, NULL);

    const unsigned int blockSize = (width/numThreads)*(height/numThreads);
    draw(height, width, c, f, rt, blockSize);

	gettimeofday(&end, NULL);

	float sec = end.tv_sec - start.tv_sec;
	sec += (end.tv_usec - start.tv_usec) / 1e6;
	cerr << sec << "s" << endl;

    SDL_EnableKeyRepeat(3,3);

	SDL_Event e;
	while(true){
		SDL_WaitEvent(&e);
		switch(e.type){
			case SDL_KEYDOWN:
				if(e.key.state == SDL_PRESSED || e.key.state == SDL_KEYDOWN){
                    switch(e.key.keysym.sym){
                        case 'q':
                            if((e.key.keysym.mod & KMOD_CTRL) || (e.key.keysym.mod & KMOD_META)){
                                return 0;
                            }
                            break;
                        case 'w':
                            c.move(vec3(0.f,0.f,-0.3f));
                            draw(height, width, c, f, rt, blockSize);
                            break;
                        case 'z':
                            c.move(vec3(0.f,-0.3f,0.f));
                            draw(height, width, c, f, rt, blockSize);
                            break;
                        case 's':
                            c.move(vec3(0.f,0.f,0.3f));
                            draw(height, width, c, f, rt, blockSize);
                            break;
                        case 'x':
                            c.move(vec3(0.f,0.3f,0.f));
                            draw(height, width, c, f, rt, blockSize);
                            break;
                        case 'a':
                            c.move(vec3(-0.3f,0.f,0.f));
                            draw(height, width, c, f, rt, blockSize);
                            break;
                        case 'd':
                            c.move(vec3(0.3f,0.f,0.f));
                            draw(height, width, c, f, rt, blockSize);
                            break;
                        default:
                            break;
                    }
				}
				break;
			case SDL_QUIT:
				return 0;
				break;
		}
	}
	return 0;
}

void draw(const int height, const int width, const camera& c, sdlFramebuffer& f, const rayTracer& rt, const unsigned int blockSize){
    const unsigned int spp = 8;
    const float invspp = 1.f / (float)spp;
    for(unsigned int i=0; i<spp; ++i){
#ifdef RT_MULTITHREADED
#pragma omp parallel for collapse(2) schedule(dynamic, blockSize)
#endif
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                //f.drawPixel(x, y, rt.L(c.getRay(x,y)));
                ///*
                rgbColor L(0.f);
                    L += rt.L(c.getRay((float)x + sampleUniform() - 0.5, (float)y + sampleUniform() - 0.5));
                    //L += rt.L(c.getRay(x,y));

                const color& prev = f.getPixel(x, y);
                f.drawPixel(x, y, rgbColor(prev.red(),prev.green(),prev.blue()) + L*invspp);
            }
        }
        f.flip();
	}
}
