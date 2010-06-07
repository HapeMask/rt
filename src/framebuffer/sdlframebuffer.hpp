#pragma once

#include <SDL.h>

#include "framebuffer.hpp"
#include "color/color.hpp"

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

        ~sdlFramebuffer();

        void drawPixel(const int& x, const int& y, const color& c);
        inline virtual const color& getPixel(const int& x, const int& y) const{
            return buffer[(y * width()) + x];
        }

		const bool readyForDrawing() const{
			return didInit;
		}

		void flip(){
            for(int y=0; y<height(); y++){
                for(int x=0; x<width(); x++){
                    setPixel(x, y, buffer[(y * width()) + x]);
                }
            }

			SDL_UpdateRect(screen, 1, 1, width_, height_);
			SDL_Flip(screen);
		}

        void setLinearTonemapScale(const float& scale){
            linearTonemapScale = scale;
        }

        void tonemapAndFlip();
	private:
		void setPixel(const int& x, const int& y, const color& c);

		SDL_Surface* screen;
		bool didInit;
        rgbColor* buffer, *tempBuffer;
        float linearTonemapScale;
};
