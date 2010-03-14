#ifndef __RT_FRAMEBUFFER__
#define __RT_FRAMEBUFFER__

#include "color.hpp"

class framebuffer {
	public:
		framebuffer(const int& w, const int& h, const int& b){
			width_ = w;
			height_ = h;
			bpp_ = b;
		}

		virtual void drawPixel(const int& x, const int& y, const color& c) = 0;
		virtual const bool readyForDrawing() const = 0;

		const int& width() const{
			return width_;
		}

		const int& height() const{
			return height_;
		}

		const int& bpp() const{
			return bpp_;
		}

	protected:
		int width_;
		int height_;
		int bpp_;
};

#endif
