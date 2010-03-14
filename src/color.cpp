#include "color.hpp"

const rgbColor rgbColor::inverse() const{
	rgbColor c(*this);
	c.invert();
	return c;
}

void rgbColor::invert(){
	r_ = 1.f - r_;
	g_ = 1.f - g_;
	b_ = 1.f - b_;
}

