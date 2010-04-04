#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>

#include "framebuffer/sdlframebuffer.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"
#include "camera/camera.hpp"

#include "scene/scene.hpp"
#include "geometry/triangle.hpp"
#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"

SDL_Surface* screen;

int main(int argc, char* argv[]){
	sdlFramebuffer f(512, 512, 32);

	scene s;
	shape sh;
	sh.addPrimitive(new plane(vec3(0,1,0), point3(0,0,0)));

	s.addShape(sh);
	s.build();

	float screen[4] = {-1, 1, -1, 1};
	camera c(512, 512, screen, 0.1f, 100.f, 45.f, point3(0,3.f,-10), point3(0,0,0), point3(0,1,0));

	ray r;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	for(int y=0; y<512; y++){
		for(int x=0; x<512; x++){
			c.getRay(x, y, r);
			const intersection i = s.intersect1(r);
			if(i.hit){
				f.drawPixel(x, y,
						i.s->getMaterial()->sample(point3(0,0,0), vec3(0,0,0), vec3(0,0,0)));
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
