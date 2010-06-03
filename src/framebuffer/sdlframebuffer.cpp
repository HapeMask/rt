#include <iostream>
#include <cmath>
using namespace std;

#include "sdlframebuffer.hpp"
#include "color/color.hpp"

sdlFramebuffer::sdlFramebuffer(const int& width, const int& height, const int& bpp):
	framebuffer(width, height, bpp){

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cerr << "Error loading SDL video:" << endl;
		cerr << SDL_GetError() << endl;
		didInit = false;
		return;
	}

	screen = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(screen == NULL){
		cerr << "Unable to obtain a screen context w/dimensions "
			<< width << "x" << height << "@" << bpp << "bpp." << endl;
		didInit = false;
		return;
	}

    buffer = new rgbColor[width*height];
	didInit = true;
}

sdlFramebuffer::~sdlFramebuffer(){
    delete[] buffer;
}

void sdlFramebuffer::drawPixel(const int& x, const int& y, const color& c){
    buffer[(width() * y) + x] = c;
}

void sdlFramebuffer::setPixel(const int& x, const int& y, const color& c){
	if(SDL_MUSTLOCK(screen)){
		if(SDL_LockSurface(screen) < 0){
			return;
		}
	}

    const float gamma = 1.f/2.2f;
    const rgbColor gammaC = rgbColor(
            powf(c.red(), gamma),
            powf(c.green(), gamma),
            powf(c.blue(), gamma));

	const uint32_t color = SDL_MapRGB(screen->format, gammaC.R(), gammaC.G(), gammaC.B());
	switch(screen->format->BytesPerPixel){
		case 1:
			{
				uint8_t* buf = (uint8_t*)screen->pixels + y*screen->pitch + x;
				*buf = color;
			}
			break;
		case 2:
			{
				uint16_t* buf = (uint16_t*)screen->pixels + y*screen->pitch/2 + x;
				*buf = color;
			}
			break;
		case 3:
			{
				uint8_t* buf = (uint8_t*)screen->pixels + y*screen->pitch + x * 3;
				if(SDL_BYTEORDER == SDL_LIL_ENDIAN){
					buf[0] = color;
					buf[1] = color >> 8;
					buf[2] = color >> 16;
				}else{
					buf[2] = color;
					buf[1] = color >> 8;
					buf[0] = color >> 16;
				}
			}
			break;
		case 4:
			{
				uint32_t* buf = (uint32_t*)screen->pixels + y*screen->pitch/4 + x;
				*buf = color;
			}
			break;
	}

	if(SDL_MUSTLOCK(screen)){
		SDL_UnlockSurface(screen);
	}
}
