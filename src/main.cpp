#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>

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

SDL_Surface* screen;

int main(int argc, char* argv[]){
	sdlFramebuffer f(512, 512, 32);

	scene s;
	shape* sh = new shape();
	shape* sh1 = new shape();
	shape* sh2 = new shape();
	shape* sh3 = new shape();
	shape* sh4 = new shape();
	shape* sh5 = new shape();

	sh->addPrimitive(new triangle(point3(-0.5,2.5,0.5), point3(0.5,2.5,-0.5), point3(0.5,2.5,0.5)));
	sh->addPrimitive(new triangle(point3(-0.5,2.5,0.5), point3(-0.5,2.5,-0.5), point3(0.5,2.5,-0.5)));
	sh->setMaterial(new material(rgbColor(1,1,1), 10.0f));

	sh1->addPrimitive(new plane(vec3(1,0,0), -3.f));
	sh1->setMaterial(new material(new lambertianBrdf(rgbColor(0,0,1))));

	sh2->addPrimitive(new plane(vec3(-1,0,0), -3.f));
	sh2->setMaterial(new material(new lambertianBrdf(rgbColor(0,1,0))));

	sh3->addPrimitive(new plane(vec3(0,1,0), -3.f));
	sh3->setMaterial(new material(new lambertianBrdf(rgbColor(1,1,1))));

	sh4->addPrimitive(new plane(vec3(0,-1,0), -3.f));
	sh4->setMaterial(new material(new lambertianBrdf(rgbColor(1,1,1))));

	sh5->addPrimitive(new sphere(point3(-1.0f, 0,0), 0.5f));
	sh5->setMaterial(new material(new lambertianBrdf(rgbColor(1,1,1))));

	s.addEmitter(sh);
	s.addShape(sh1);
	s.addShape(sh2);
	s.addShape(sh3);
	s.addShape(sh4);
	s.addShape(sh5);
	s.addLight(new pointLight(vec3(-300,300,-300), rgbColor(1.f,1.f,1.f)));
	s.build();

	float screen[4] = {-1, 1, -1, 1};
	camera c(512, 512, screen, 0.1f, 100.f, 45.f, point3(0,1.f,-10), point3(0,0,0), point3(0,1,0));

	ray r;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	whittedRayTracer rt(&s);

	srand(time(NULL));
	/*
	c.getRay(150,230,r);
	rt.L(r);
	return 0;
	*/

	for(int y=0; y<512; y++){
		for(int x=0; x<512; x++){
			c.getRay(x, y, r);
			f.drawPixel(x, y, rt.L(r));
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
