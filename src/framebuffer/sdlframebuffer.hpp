#pragma once

#include "framebuffer.hpp"
#include "color/color.hpp"
#include "scene/scene.hpp"

#include <SDL.h>
#include <omp.h>
#include <vector>
using namespace std;

// Number of image blocks in the X- and Y- axes.
static const int HORIZ_BLOCKS = 8;
static const int VERT_BLOCKS = 8;

class sdlFramebuffer : public framebuffer {
	public:
		sdlFramebuffer(const scene& sc, const int bpp = 32);
        ~sdlFramebuffer();

        virtual const bool render();

		const bool readyForDrawing() const{
			return didInit;
		}

        void setLinearTonemapScale(const float& scale){
            linearTonemapScale = scale;
            tonemapAndUpdateScreen();
        }

        inline void setShowUpdates(const bool show){
            showUpdates = show;
        }

	private:
		void addSample(const int& x, const int& y, const rgbColor& c);
		void setPixel(const int& x, const int& y, const rgbColor& c);

        void tonemapAndUpdateScreen();
        void tonemapAndUpdateRect(const int& cornerX, const int& cornerY);

        /*
         * Generates upper-left coordinates for the next block in the sequence of image
         * blocks.
         */
        inline const bool getNextBlock(int& x, int& y){
            bool done = false;

            // Critical section protects race conditions on blocksUsed.
#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
            {
                x = (blocksUsed % HORIZ_BLOCKS) * blockWidth;
                y = (blocksUsed / HORIZ_BLOCKS) * blockHeight;
                ++blocksUsed;

                done = (blocksUsed > (HORIZ_BLOCKS * VERT_BLOCKS));
            }

            return done;
        }

        int blocksUsed;
        uint64_t pixelsSampled, iterations;
        const scene& scn;
        const int blockWidth, blockHeight;

		SDL_Surface* screen;
		bool didInit, showUpdates;
        rgbColor* buffer;
        rgbColor* sumOfSquares;
        int* samplesPerPixel;
        float linearTonemapScale;
};
