#include "mathlib/constants.hpp"
#include "sdlframebuffer.hpp"
#include "color/color.hpp"

#include "utility.hpp"

#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <unistd.h>

#include "omp.h"

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

	screen = SDL_SetVideoMode(width_, height_, bpp, SDL_HWSURFACE);// | SDL_DOUBLEBUF);
	if(screen == NULL){
		cerr << "Unable to obtain a screen context w/dimensions "
			<< width_ << "x" << height_ << "@" << bpp << "bpp." << endl;
		didInit = false;
		return;
	}

    //buffer = new vector<rgbColor>[width_*height_];
    buffer = new rgbColor[width_*height_];

	didInit = true;
}

sdlFramebuffer::~sdlFramebuffer(){
    delete[] buffer;
}

void sdlFramebuffer::addSample(const int& x, const int& y, const color& c){
    //buffer[(width() * y) + x].push_back(c);
    buffer[(width() * y) + x] += c;
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

void sdlFramebuffer::render(){
    struct timeval start, now;
    gettimeofday(&start, NULL);

#ifdef RT_MULTITHREADED
#pragma omp parallel
#endif
    {
        // Fill blocks as long as the thread can get new ones.
        int blockCornerX=0, blockCornerY=0;
        while(!getNextBlock(blockCornerX,blockCornerY)){

            if(showUpdates){
                const uint32_t rectColor = SDL_MapRGB(screen->format, 0,200,0);
                SDL_Rect updatedRect = { blockCornerX, blockCornerY, blockWidth, blockHeight };

#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
                {
                    SDL_FillRect(screen, &updatedRect, rectColor);
                    SDL_UpdateRect(screen, blockCornerX, blockCornerY, blockWidth, blockHeight);
                }
            }

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
#ifdef RT_MULTITHREADED
#pragma omp atomic
#endif
                    ++samplesTaken;
                }
            }

            tonemapAndUpdateRect(blockCornerX, blockCornerY);
        }
    }

    gettimeofday(&now, NULL);
    const float timeElapsed = now.tv_sec - start.tv_sec +
        ((now.tv_usec - start.tv_usec) / 1e6);

    ++spp;
    cerr << "SPP: " << spp << ", ";
    cerr << "samples/sec: " << (float)(width_*height_)/timeElapsed << endl;

    blocksUsed = 0;
}

void sdlFramebuffer::tonemapAndUpdateScreen(){
#ifdef RT_MULTITHREADED
#pragma omp parallel for collapse(2)
#endif
    for(int y=0; y<height(); y++){
        for(int x=0; x<width(); x++){
            setPixel(x, y, clamp(buffer[y * width() + x] / (float)spp));
        }
    }

    SDL_UpdateRect(screen, 1, 1, width_, height_);
}

void sdlFramebuffer::tonemapAndUpdateRect(const int& cornerX, const int& cornerY){
    for(int y = cornerY; y < cornerY + blockHeight; y++){
        for(int x = cornerX; x < cornerX + blockWidth; x++){
            setPixel(x, y, clamp(buffer[y * width() + x] / (float)spp));
        }
    }

#ifdef RT_MULTITHREADED
#pragma omp critical
#endif
    {
        SDL_UpdateRect(screen, cornerX, cornerY, blockWidth, blockHeight);
    }
}
