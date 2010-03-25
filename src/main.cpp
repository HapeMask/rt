#include <SDL/SDL.h>
#include <iostream>

#include "framebuffer/sdlframebuffer.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"
#include "camera/camera.hpp"

#include "geometry/triangle.hpp"
#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"
using namespace std;

SDL_Surface* screen;

int main(int argc, char** argv){
	sdlFramebuffer f(512, 512, 32);

	plane pl(vec3(0,1,0), point3(0,0,0));

	float screen[4] = {-1, 1, -1, 1};
	camera c(512, 512, screen, 0.1f, 100.f, 45.f, point3(0,1,-10), point3(0,0,0), point3(0,1,0));

	ray r;
	point3 p;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	for(int y=0; y<512; y++){
		for(int x=0; x<512; x++){
			c.getRay(x, y, r);
			if(pl.intersect(r, p)){
				f.drawPixel(x, y, white);
			}
		}
	}

	f.flip();

	cerr << "done" << endl;

	SDL_Event e;
	while(true){
		SDL_WaitEvent(&e);
		switch(e.type){
			case SDL_KEYDOWN:
				cerr << "pressed: " << SDL_GetKeyName(e.key.keysym.sym) << endl;
				break;
			case SDL_QUIT:
				exit(0);
				break;
		}
	}
	return 0;
}
