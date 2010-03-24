#include <SDL/SDL.h>
#include <iostream>

#include "framebuffer/sdlframebuffer.hpp"
#include "geometry/triangle.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"
#include "camera/camera.hpp"
using namespace std;

SDL_Surface* screen;

int main(int argc, char** argv){
	sdlFramebuffer f(512, 512, 32);

	triangle t1(point3(0,0,0), point3(0,1,0), point3(-1,1,0));
	triangle t2(point3(0,0,0), point3(-1,1,0), point3(-1,0,0));

	camera c(512, 512, 45.f, point3(0,0.5,-10), point3(0,0,0), point3(0,1,0));

	float v[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,1,0}
	};
	mat4 m(v[0]);
	vec4 v(0,0,3,1);
	cerr << (m * v) << endl;
	return 0;

	ray r;
	point3 p;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	for(int y=0; y<512; y++){
		for(int x=0; x<512; x++){
			c.getRay(x, y, r);

			if(t1.intersect(r, p)){
				f.drawPixel(x, y, white);
			}else{
				f.drawPixel(x, y, black);
			}

			c.getRay(x, y, r);
			if(t2.intersect(r, p)){
				f.drawPixel(x, y, blue);
			}
		}
	}

	f.flip();

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
