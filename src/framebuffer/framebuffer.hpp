#pragma once
#include "scene/scene.hpp"

// Number of image blocks in the X- and Y- axes.
static const int HORIZ_BLOCKS = 4;
static const int VERT_BLOCKS = 4;

class framebuffer {
	public:
		framebuffer(scene& sc, const int& b) :
            fb_width(sc.getCamera().width()), fb_height(sc.getCamera().height()),
            fb_bpp(b), scn(sc),
            blockWidth(sc.getCamera().width() / HORIZ_BLOCKS),
            blockHeight(sc.getCamera().height() / VERT_BLOCKS),
            blocksUsed(0)
        {}

        virtual ~framebuffer() {}
        virtual void render() = 0;

	protected:
        /*
         * Generates upper-left coordinates for the next block in the sequence of image
         * blocks.
         */
        inline bool getNextBlock(int& x, int& y){
            bool done = false;

            // Critical section protects against race conditions on blocksUsed.
#ifdef RT_USE_OPENMP
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

		int fb_width, fb_height, fb_bpp;

        scene& scn;

        const int blockWidth, blockHeight;
        int blocksUsed;

        virtual void addSample(const int& x, const int& y, const rgbColor& c) = 0;
        virtual void setPixel(const int& x, const int& y, const rgbColor& c) = 0;
};
