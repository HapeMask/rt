#pragma once

#include "framebuffer.hpp"
#include "color/color.hpp"
#include "scene/scene.hpp"

#include <SDL.h>
#include <omp.h>
#include <vector>
using namespace std;

class sdlFramebuffer : public framebuffer {
	public:
		sdlFramebuffer(scene& sc, const int bpp = 32);
        ~sdlFramebuffer();

        virtual void render();

		const bool readyForDrawing() const{
			return didInit;
		}

        inline void setShowUpdates(const bool show){
            showUpdates = show;
        }

    protected:
		virtual void addSample(const int& x, const int& y, const rgbColor& c);
		virtual void setPixel(const int& x, const int& y, const rgbColor& c);

	private:

        void tonemapAndUpdateScreen();
        void tonemapAndUpdateRect(const int& cornerX, const int& cornerY);

        uint64_t pixelsSampled, iterations;

		SDL_Surface* screen;
		bool didInit, showUpdates;
        rgbColor* buffer;
        rgbColor* sumOfSquares;
        int* samplesPerPixel;
};
