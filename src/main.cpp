#include <omp.h>
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
#include "geometry/plane.hpp"

#include "materials/material.hpp"
#include "materials/brdf.hpp"
#include "color/color.hpp"
#include "light/light.hpp"

#include "tracer/tracer.hpp"
#include "samplers/samplers.hpp"
#include "scene/parser.hpp"
#include "acceleration/bvh.hpp"

SDL_Surface* screen;

// Default to 8 threads.
#ifndef RT_OMP_THREADS
#define RT_OMP_THREADS 8
#endif

int main(int argc, char* args[]){
	scene s;

    string filename;

    if(argc < 2){
        filename = "../src/scene/test.scn";
    }else{
        filename = string(args[1]);
    }

	ifstream in(filename.c_str());
	sceneParser p;
	p.parse(s, in);
	in.close();

	s.build();

	const cameraPtr& c = s.getCamera();
    const int width = c->width();
    const int height = c->height();
	sdlFramebuffer f(width, height, 32);

	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	whittedRayTracer rt(&s);

	srand(time(NULL));

	struct timeval start, end;
	gettimeofday(&start, NULL);

    omp_set_num_threads(RT_OMP_THREADS);
    cerr << "Rendering on " << RT_OMP_THREADS << " threads." << endl;

#pragma omp parallel for
	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			f.drawPixel(x, y, rt.L(c->getRay(x,y)));
		}
	}
	gettimeofday(&end, NULL);

	f.flip();

	float sec = end.tv_sec - start.tv_sec;
	sec += (end.tv_usec - start.tv_usec) / 1e6;
	cerr << sec << "s" << endl;

	SDL_Event e;
	while(true){
		SDL_WaitEvent(&e);
		switch(e.type){
			case SDL_KEYDOWN:
				if(e.key.state == SDL_PRESSED || e.key.state == SDL_KEYDOWN){
					if(e.key.keysym.sym == 'q' && (
							(e.key.keysym.mod & KMOD_CTRL) || (e.key.keysym.mod & KMOD_META) )){
						exit(0);
					}
				}
				break;
			case SDL_QUIT:
				exit(0);
				break;
		}
	}
	return 0;
}
