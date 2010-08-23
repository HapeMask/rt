#include "mathlib/constants.hpp"
#include "sdlframebuffer.hpp"

#include "utility.hpp"

#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

sdlFramebuffer::sdlFramebuffer(const scene& sc, const int bpp):
    framebuffer(sc.getCamera().width(), sc.getCamera().height(), bpp),
    blocksUsed(0), samplesTaken(0), spp(0), scn(sc),
    blockWidth(sc.getCamera().width() / HORIZ_BLOCKS),
    blockHeight(sc.getCamera().height() / VERT_BLOCKS), didInit(false),
    showUpdates(false), linearTonemapScale(1.f)
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
    spps = new int[width_*height_];

	didInit = true;
}

sdlFramebuffer::~sdlFramebuffer(){
    delete[] buffer;
    delete[] sumOfSquares;
    delete[] spps;
}

void sdlFramebuffer::addSample(const int& x, const int& y, const rgbColor& c){
    const size_t offset = y * width_ + x;
    buffer[offset] += c;
    sumOfSquares[offset] += c * c;
    ++spps[offset];

#ifdef RT_MULTITHREADED
#pragma omp atomic
#endif
            ++samplesTaken;
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

const bool sdlFramebuffer::render(){
    struct timeval start, now;
    gettimeofday(&start, NULL);


    bool done = false;
#ifdef RT_MULTITHREADED
#pragma omp parallel for schedule(dynamic)
#endif
    // Fill blocks as long as the thread can get new ones.
    for(int i=0; i<(HORIZ_BLOCKS * VERT_BLOCKS); ++i){
        int blockCornerX=0, blockCornerY=0;
        getNextBlock(blockCornerY, blockCornerX);

        if(showUpdates){
            const uint32_t rectColor = SDL_MapRGB(screen->format, 0,200,0);
            SDL_Rect updatedRect = { blockCornerX, blockCornerY, blockWidth, blockHeight };
            SDL_FillRect(screen, &updatedRect, rectColor);

#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
            {
                SDL_UpdateRect(screen, blockCornerX, blockCornerY, blockWidth, blockHeight);
            }
        }

        // Render the pixels in block.
        for(int y=blockCornerY; y< blockCornerY + blockHeight; ++y){
            for(int x=blockCornerX; x < blockCornerX + blockWidth; ++x){
                // TODO: Replace this basic jittering with improved filtering,
                // perhaps stratification over the image plane.
                //
                // Also look into
                // adaptive sampling based on z-test.
                const float xOffset = sampleUniform() - 0.5f;
                const float yOffset = sampleUniform() - 0.5f;
                addSample(
                        x, y,
                        scn.L((float)x + xOffset, (float)y + yOffset)
                    );
            }
        }

        //tonemapAndUpdateRect(blockCornerX, blockCornerY);
    }
    tonemapAndUpdateScreen();

    gettimeofday(&now, NULL);
    const float timeElapsed = now.tv_sec - start.tv_sec +
        ((now.tv_usec - start.tv_usec) / 1e6);

    ++spp;
    cerr << "SPP: " << spp << ", ";
    cerr << "samples/sec: " << (float)(width_*height_)/timeElapsed << endl;

    blocksUsed = 0;
    return done;
}

void sdlFramebuffer::tonemapAndUpdateScreen(){
#ifdef RT_MULTITHREADED
#pragma omp parallel for
#endif
    for(int y = 0; y <height_; y++){
        for(int x = 0; x < width_; x++){
            setPixel(x, y, clamp(buffer[y * width() + x] / (float)spps[y * width_ + x]));
        }
    }

    SDL_UpdateRect(screen, 0, 0, width_, height_);
}

void sdlFramebuffer::tonemapAndUpdateRect(const int& cornerX, const int& cornerY){
    for(int y = cornerY; y < cornerY + blockHeight; y++){
        for(int x = cornerX; x < cornerX + blockWidth; x++){
            setPixel(x, y, clamp(buffer[y * width() + x] / (float)spps[y * width_ + x]));
        }
    }

#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
        {
            SDL_UpdateRect(screen, cornerX, cornerY, blockWidth, blockHeight);
        }
}
