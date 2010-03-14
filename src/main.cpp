#include <SDL/SDL.h>
#include <iostream>

#include "framebuffer/sdlframebuffer.hpp"
#include "geometry/triangle.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "camera/camera.hpp"
using namespace std;

SDL_Surface* screen;

int main(int argc, char** argv){
	sdlFramebuffer f(512, 512, 32);
	triangle t(point3(0,0,0), point3(1,0,0), point3(0,1,0));
	camera c(512, 512, 45, vec3(0,1,0), vec3(0,0,1), point3(0.5, 0.5, -2));
	ray r;
	point3 p;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);

	for(int y=1; y<=512; y++){
		for(int x=1; x<=512; x++){
			c.getRay(x, y, r);
			if(t.intersect(r, p)){
				f.drawPixel(x, y, white);
			}else{
				f.drawPixel(x, y, black);
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
