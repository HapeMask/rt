#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>

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
	/*
	shapePtr sh(new shape());
	shapePtr sh1(new shape());

	sh->addPrimitive(primitivePtr(new plane(vec3(1,0,0), -3.f)));
	sh->setMaterial(materialPtr(new material(brdfPtr(new lambertianBrdf(rgbColor(1,1,1))))));

	sh1->addPrimitive(primitivePtr(new sphere(point3(-1.0f, 0,0), 0.5f)));
	sh1->setMaterial(materialPtr(new material(brdfPtr(new lambertianBrdf(rgbColor(1,1,1))))));

	s.addShape(sh);
	s.addShape(sh1);
	*/

	s.addLight(lightPtr(new pointLight(point3(0,0,0), rgbColor(1.f,1.f,1.f), 5.0f)));

	ifstream in("../src/scene/test.scn");
	sceneParser p;
	p.parse(s, in);
	in.close();

	s.build();

	float screen[4];
	if(width > height){
		const float ratio = (float)width / (float)height;
		screen[0] = -ratio;
		screen[1] = ratio;
		screen[2] = -1;
		screen[3] = 1;
	}else{
		const float ratio = (float)height / (float)width;
		screen[0] = -1;
		screen[1] = 1;
		screen[2] = -ratio;
		screen[3] = ratio;
	}
	camera c(width, height, screen, 0.1f, 100.f, 45.f, point3(0,0,-10), point3(0,0,0), point3(0,1,0));

	ray r0, r1, r2, r3, r4;
	rgbColor white(1,1,1);
	rgbColor black(0,0,0);
	rgbColor blue(0,0,1);

	whittedRayTracer rt(&s);

	srand(time(NULL));

	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			c.getRay(x, y, r0);
			c.getRay(x+0.25f, y+0.25f, r1);
			c.getRay(x-0.25f, y+0.25f, r2);
			c.getRay(x+0.25f, y-0.25f, r3);
			c.getRay(x-0.25f, y-0.25f, r4);
			f.drawPixel(x, y,
					(rt.L(r0) + rt.L(r1) + rt.L(r2) + rt.L(r3) + rt.L(r4)) /5.f
					);
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
