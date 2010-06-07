#include <iostream>
#include <cmath>
using namespace std;

#include "mathlib/constants.hpp"
#include "sdlframebuffer.hpp"
#include "color/color.hpp"

sdlFramebuffer::sdlFramebuffer(const int& width, const int& height, const int& bpp):
	framebuffer(width, height, bpp), linearTonemapScale(1.f){

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
    tempBuffer = new rgbColor[width*height];
	didInit = true;
}

sdlFramebuffer::~sdlFramebuffer(){
    delete[] buffer;
    delete[] tempBuffer;
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

void sdlFramebuffer::tonemapAndFlip(){
    memcpy((void*)tempBuffer, (void*)buffer, width()*height()*sizeof(rgbColor));
    /*
    float cMax = MIN_FLOAT;

    for(int y=0; y<height(); y++){
        for(int x=0; x<width(); x++){
            const unsigned int i = y * width() + x;
            const rgbColor& c = tempBuffer[i];

            const float cm = max(max(c.r, c.g), c.b);
            if(cm > cMax) cMax = cm;
        }
    }

    for(int y=0; y<height(); y++){
        for(int x=0; x<width(); x++){
            tempBuffer[y * width() + x] /= cMax;
        }
    }
    */

    for(int y=0; y<height(); y++){
        for(int x=0; x<width(); x++){
            tempBuffer[y * width() + x] /= linearTonemapScale;
        }
    }

    for(int y=0; y<height(); y++){
        for(int x=0; x<width(); x++){
            setPixel(x, y, tempBuffer[(y * width()) + x]);
        }
    }

    SDL_UpdateRect(screen, 1, 1, width_, height_);
    SDL_Flip(screen);
}
