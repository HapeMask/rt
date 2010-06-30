#pragma once

#include "framebuffer.hpp"
#include "color/color.hpp"
#include "scene/scene.hpp"

#include <SDL.h>
#include <vector>
using namespace std;

class sdlFramebuffer : public framebuffer {
	public:
		sdlFramebuffer(const scene& sc, const int bpp = 32);
        ~sdlFramebuffer();

        virtual void render();

		const bool readyForDrawing() const{
			return didInit;
		}

        void setLinearTonemapScale(const float& scale){
            linearTonemapScale = scale;
            tonemapAndFlip();
        }

	private:
		void addSample(const int& x, const int& y, const color& c);
		void setPixel(const int& x, const int& y, const color& c);
        void tonemapAndFlip();


        uint64_t samplesTaken;
        const scene& scn;

		SDL_Surface* screen;
		bool didInit;
        vector<rgbColor>* buffer;
        float linearTonemapScale;
};
