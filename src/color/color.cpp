#include "color.hpp"
#include <iostream>
using namespace std;

rgbColor::rgbColor(const float& r, const float& g, const float& b) : r(r), g(g), b(b)
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

const rgbColor rgbColor::operator*(const float& f) const{
	return rgbColor(*this) *= f;
}

rgbColor& rgbColor::operator*=(const float& f){
	r *= f;
	g *= f;
	b *= f;
	return (*this);
}

const rgbColor rgbColor::operator/(const float& f) const{
	return rgbColor(*this) *= (1.f / f);
}

rgbColor& rgbColor::operator/=(const float& f){
	return this->operator*=(1.f/f);
}

const rgbColor rgbColor::operator-(const rgbColor& c) const{
	return rgbColor(*this) -= c;
}

rgbColor& rgbColor::operator-=(const rgbColor& c){
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return (*this);
}

const rgbColor rgbColor::operator+(const rgbColor& c) const{
	return rgbColor(*this) += c;
}

rgbColor& rgbColor::operator+=(const rgbColor& c){
	r += c.r;
	g += c.g;
	b += c.b;
	return (*this);
}

const rgbColor rgbColor::operator*(const rgbColor& c) const{
	return rgbColor(*this) *= c;
}

rgbColor& rgbColor::operator*=(const rgbColor& c){
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return (*this);
}

ostream& operator<<(ostream& out, const color& c){
	out << "color(" << c.red() << ", " << c.green() << ", " << c.blue() << ")";
	return out;
}
