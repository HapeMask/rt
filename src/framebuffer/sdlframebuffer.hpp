#ifndef __RT_SDLFRAMEBUFFER__
#define __RT_SDLFRAMEBUFFER__

#include <SDL/SDL.h>

#include "framebuffer.hpp"
#include "../color/color.hpp"

class sdlFramebuffer : public framebuffer {
	public:
		sdlFramebuffer(const int& width, const int& height, const int& bpp);

		sdlFramebuffer(SDL_Surface* p) :
			framebuffer(p->w, p->h, p->format->BitsPerPixel){

			screen = p;
			if(p == NULL){
				didInit = false;
			}else{
				didInit = true;
			}
		}

		void drawPixel(const int& x, const int& y, const color& c);

		const bool readyForDrawing() const{
			return didInit;
		}

		void flip(){
			SDL_UpdateRect(screen, 1, 1, width_, height_);
			SDL_Flip(screen);
		}

	private:
		SDL_Surface* screen;
		bool didInit;
};
#endif
