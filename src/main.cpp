#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>
#include <limits>

#include "utility.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"

#include "framebuffer/sdlframebuffer.hpp"
#include "camera/camera.hpp"
#include "scene/scene.hpp"

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

SDL_Surface* screen;

const int width = 512;
const int height = 384;

int main(int argc, char* argv[]){
	sdlFramebuffer f(width, height, 32);

	scene s;

	ifstream in("../src/scene/test.scn");
	sceneParser p;
	p.parse(s, in);
	in.close();

	s.build();

	camera c(width, height, 0.1f, 100.f, 45.f, point3(0,0,-10), point3(0,0,0), point3(0,1,0));

	ray r0, r1, r2, r3, r4;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	whittedRayTracer rt(&s);

	srand(time(NULL));

	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			c.getRay(x, y, r0);
			f.drawPixel(x, y, rt.L(r0));
		}
	}

	f.flip();

	cerr << "done" << endl;

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
