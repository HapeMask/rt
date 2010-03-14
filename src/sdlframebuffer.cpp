#include <iostream>
using namespace std;

#include "sdlframebuffer.hpp"
#include "color.hpp"

sdlFramebuffer::sdlFramebuffer(const int& width, const int& height, const int& bpp):
	framebuffer(width, height, bpp){

	cerr << "Loading SDL video..." << endl;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cerr << "Error loading SDL video:" << endl;
		cerr << SDL_GetError() << endl;
		didInit = false;
		return;
	}
	cerr << "SDL video loaded." << endl;

	screen = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(screen == NULL){
		cerr << "Unable to obtain a screen context w/dimensions 512x512, 32bpp." << endl;
		didInit = false;
		return;
	}

	didInit = true;
}

void sdlFramebuffer::drawPixel(const int& x, const int& y, const color& c){
	if(SDL_MUSTLOCK(screen)){
		if(SDL_LockSurface(screen) < 0){
			return;
		}
	}

	uint32_t color = SDL_MapRGB(screen->format, c.R(), c.G(), c.B());
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
