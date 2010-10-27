#ifdef RT_USE_SDL

#include "mathlib/constants.hpp"
#include "sdlframebuffer.hpp"

#include "utility.hpp"

#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

sdlFramebuffer::sdlFramebuffer(scene& sc, const int bpp):
    framebuffer(sc, bpp),
    pixelsSampled(0), iterations(0), showUpdates(false)
{

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cerr << "Error loading SDL video:" << endl;
		cerr << SDL_GetError() << endl;
		didInit = false;
		return;
	}

	screen = SDL_SetVideoMode(width_, height_, bpp, SDL_HWSURFACE);
	if(screen == NULL){
		cerr << "Unable to obtain a screen context w/dimensions "
			<< width_ << "x" << height_ << "@" << bpp << "bpp." << endl;
		didInit = false;
		return;
	}

    buffer = new rgbColor[width_*height_];
    sumOfSquares = new rgbColor[width_*height_];
    samplesPerPixel = new int[width_*height_];

	didInit = true;
}

sdlFramebuffer::~sdlFramebuffer(){
    if(buffer) delete[] buffer;
    if(sumOfSquares) delete[] sumOfSquares;
    if(samplesPerPixel) delete[] samplesPerPixel;
}

void sdlFramebuffer::addSample(const int& x, const int& y, const rgbColor& c){
    const size_t offset = y * width_ + x;
    buffer[offset] += c;
    sumOfSquares[offset] += c * c;
    ++samplesPerPixel[offset];

#ifdef RT_MULTITHREADED
#pragma omp atomic
#endif
            ++pixelsSampled;
}

void sdlFramebuffer::setPixel(const int& x, const int& y, const rgbColor& c){
	if(SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0){
        return;
	}

    const float gamma = 1.f/2.2f;
    const rgbColor gammaC = rgbColor(
            powf(c.red(), gamma),
            powf(c.green(), gamma),
            powf(c.blue(), gamma));

	const uint32_t rgbColor = SDL_MapRGB(screen->format, gammaC.R(), gammaC.G(), gammaC.B());
	switch(screen->format->BytesPerPixel){
		case 1:
			{
				uint8_t* buf = (uint8_t*)screen->pixels + y*screen->pitch + x;
				*buf = rgbColor;
			}
			break;
		case 2:
			{
				uint16_t* buf = (uint16_t*)screen->pixels + y*screen->pitch/2 + x;
				*buf = rgbColor;
			}
			break;
		case 3:
			{
				uint8_t* buf = (uint8_t*)screen->pixels + y*screen->pitch + x * 3;
				if(SDL_BYTEORDER == SDL_LIL_ENDIAN){
					buf[0] = rgbColor;
					buf[1] = rgbColor >> 8;
					buf[2] = rgbColor >> 16;
				}else{
					buf[2] = rgbColor;
					buf[1] = rgbColor >> 8;
					buf[0] = rgbColor >> 16;
				}
			}
			break;
		case 4:
			{
				uint32_t* buf = (uint32_t*)screen->pixels + y*screen->pitch/4 + x;
				*buf = rgbColor;
			}
			break;
	}

	if(SDL_MUSTLOCK(screen)){
		SDL_UnlockSurface(screen);
	}
}

void sdlFramebuffer::render(){
    struct timeval start, now;
    gettimeofday(&start, NULL);

#ifdef RT_MULTITHREADED
#pragma omp parallel for schedule(dynamic)
#endif
    // Fill blocks as long as the thread can get new ones.
    for(int i=0; i<(HORIZ_BLOCKS * VERT_BLOCKS); ++i){
        int blockCornerX=0, blockCornerY=0;
        getNextBlock(blockCornerY, blockCornerX);

        // Render the pixels in the block.
//#pragma omp parallel for collapse(2) schedule(dynamic)
        for(int y=blockCornerY; y< blockCornerY + blockHeight; ++y){
            for(int x=blockCornerX; x < blockCornerX + blockWidth; ++x){
                const float xOffset = sampleUniform() - 0.5f;
                const float yOffset = sampleUniform() - 0.5f;
                addSample(
                        x, y,
                        scn.L((float)x + xOffset, (float)y + yOffset)
                    );
            }
        }
    }

    tonemapAndUpdateScreen();

    gettimeofday(&now, NULL);
    const float timeElapsed = now.tv_sec - start.tv_sec +
        ((now.tv_usec - start.tv_usec) / 1e6);

    ++iterations;
    cerr << "Iterations: " << iterations << ", ";
    cerr << "samples/sec: " << (float)(width_*height_)/timeElapsed << endl;
    cerr << "Time for this iteration: " << timeElapsed << endl << endl;

    blocksUsed = 0;
}

void sdlFramebuffer::tonemapAndUpdateScreen(){
#ifdef RT_MULTITHREADED
#pragma omp parallel for collapse(2)
#endif
    for(int y = 0; y <height_; y++){
        for(int x = 0; x < width_; x++){
            setPixel(x, y, clamp(buffer[y * width() + x] / (float)samplesPerPixel[y * width_ + x]));
        }
    }

    SDL_UpdateRect(screen, 0, 0, width_, height_);
}

void sdlFramebuffer::tonemapAndUpdateRect(const int& cornerX, const int& cornerY){
    for(int y = cornerY; y < cornerY + blockHeight; y++){
        for(int x = cornerX; x < cornerX + blockWidth; x++){
            setPixel(x, y, clamp(buffer[y * width() + x] / (float)samplesPerPixel[y * width_ + x]));
        }
    }

#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
        {
            SDL_UpdateRect(screen, cornerX, cornerY, blockWidth, blockHeight);
        }
}

#endif
