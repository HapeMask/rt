#include "color.hpp"
#include <iostream>
using namespace std;

rgbColor::rgbColor(const float& f) : r(f), g(f), b(f)
{}

rgbColor::rgbColor(const float& r, const float& g, const float& b) : r(r), g(g), b(b)
{}

rgbColor::rgbColor(const int& r, const int& g, const int& b) :
    r(r/255.f), g(g/255.f), b(b/255.f)
{}

rgbColor::rgbColor(const color& c) : r(c.red()), g(c.green()), b(c.blue())
{}

const rgbColor rgbColor::inverse() const{
	rgbColor c(*this);
	c.invert();
	return c;
}

void rgbColor::invert(){
	r = 1.f - r;
	g = 1.f - g;
	b = 1.f - b;
}

ostream& operator<<(ostream& out, const color& c){
	out << "color(" << c.red() << ", " << c.green() << ", " << c.blue() << ")";
	return out;
}
