#ifdef RT_MULTITHREADED
#include <omp.h>
#endif

#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>

#include <sys/time.h>
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
#include "scene/parser.hpp"
#include "acceleration/bvh.hpp"

// Default to 8 threads.
#ifndef RT_OMP_THREADS
#define RT_OMP_THREADS 8
#endif

void draw(const int height, const int width, const camera& c, sdlFramebuffer& f, const rayTracer& rt);

int main(int argc, char* args[]){
    /*
    const sphere sph(point3(0,0,0), 1);
    ray rr(point3(-10,0,11), normalize(vec3(1,0,-1)));
    const intersection isect = sph.intersect(rr);
    cerr << -rr.direction << endl;
    cerr << worldToBsdf(-rr.direction, isect.normal, isect.dpdu, isect.dpdv) << endl;
    return 0;
    */

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
        exit(1);
    }

	sceneParser p;
	p.parse(s, in);
	in.close();

	s.build();

	camera& c = *s.getCamera();
    const int width = c.width();
    const int height = c.height();

	sdlFramebuffer f(width, height, 32);

	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	whittedRayTracer rt(&s);
	srand(time(NULL));

    /*
    ray r0 = c.getRay(256,256);
    rt.L(r0);
    return 0;
    */

#ifdef RT_MULTITHREADED
    omp_set_num_threads(numThreads);
    cerr << "Rendering on " << numThreads << " threads." << endl;
#endif

	struct timeval start, end;
	gettimeofday(&start, NULL);
    draw(height, width, c, f, rt);
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
                            draw(height, width, c, f, rt);
                            break;
                        case 'z':
                            c.move(vec3(0.f,-0.3f,0.f));
                            draw(height, width, c, f, rt);
                            break;
                        case 's':
                            c.move(vec3(0.f,0.f,0.3f));
                            draw(height, width, c, f, rt);
                            break;
                        case 'x':
                            c.move(vec3(0.f,0.3f,0.f));
                            draw(height, width, c, f, rt);
                            break;
                        case 'a':
                            c.move(vec3(-0.3f,0.f,0.f));
                            draw(height, width, c, f, rt);
                            break;
                        case 'd':
                            c.move(vec3(0.3f,0.f,0.f));
                            draw(height, width, c, f, rt);
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

void draw(const int height, const int width, const camera& c, sdlFramebuffer& f, const rayTracer& rt){
#ifdef RT_MULTITHREADED
#pragma omp parallel for
#endif
	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			f.drawPixel(x, y, rt.L(c.getRay(x,y)));
		}
	}

	f.flip();
}
