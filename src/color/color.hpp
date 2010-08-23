#pragma once

#include <cmath>
#include <stdint.h>
#include <iostream>
using namespace std;

class color {
	public:
		virtual const float red() const = 0;
		virtual const float green() const = 0;
		virtual const float blue() const = 0;

		const uint8_t R() const {
			return 255.f * red();
		}

		const uint8_t G() const {
			return 255.f * green();
		}

		const uint8_t B() const {
			return 255.f * blue();
		}

        inline const float gray() const {
            return red() * 0.297f + green() * 0.569f + blue() * 0.114f;
        }

        inline const float avg() const {
            return (red() + blue() + green()) / 3.f;
        }

		virtual void invert() = 0;
};

class rgbColor : public color {
	public:
        rgbColor(const float& f);
		rgbColor() : r(0), g(0), b(0) {}
		rgbColor(const float& r, const float& g, const float& b);
		rgbColor(const color& c);

		const rgbColor inverse() const;
		virtual void invert();

		virtual const float red() const {
			return r;
		}

		virtual const float green() const {
			return g;
		}

		virtual const float blue() const {
			return b;
		}

        inline const bool isBlack() const {
            return (r <= 0 && g <= 0 && b <= 0);
        }

		inline const rgbColor operator/(const rgbColor& c) const{
            return rgbColor(*this) /= c;
        }

		inline rgbColor& operator/=(const rgbColor& c){
            r /= c.r;
            g /= c.g;
            b /= c.b;
            return (*this);
        }

        inline const rgbColor operator*(const float& f) const{
            return rgbColor(*this) *= f;
        }

        inline rgbColor& operator*=(const float& f){
            r *= f;
            g *= f;
            b *= f;
            return (*this);
        }

        inline const rgbColor operator/(const float& f) const{
            return rgbColor(*this) *= (1.f / f);
        }

        inline rgbColor& operator/=(const float& f){
            return this->operator*=(1.f/f);
        }

        const rgbColor operator-(const rgbColor& c) const{
            return rgbColor(*this) -= c;
        }

        inline rgbColor& operator-=(const rgbColor& c){
            r -= c.r;
            g -= c.g;
            b -= c.b;
            return (*this);
        }

        inline const rgbColor operator+(const rgbColor& c) const{
            return rgbColor(*this) += c;
        }

        inline rgbColor& operator+=(const rgbColor& c){
            r += c.r;
            g += c.g;
            b += c.b;
            return (*this);
        }

        inline const rgbColor operator*(const rgbColor& c) const{
            return rgbColor(*this) *= c;
        }

        inline rgbColor& operator*=(const rgbColor& c){
            r *= c.r;
            g *= c.g;
            b *= c.b;
            return (*this);
        }

		float r;
		float g;
		float b;
};

inline const rgbColor sqrt(const rgbColor& c){
    return rgbColor(sqrtf(c.r), sqrtf(c.g), sqrtf(c.b));
}

inline rgbColor& operator*=(const float& f, rgbColor& c){
    c.r *= f;
    c.g *= f;
    c.b *= f;
    return c;
}

inline const rgbColor operator*(const float& f, const rgbColor& c){
    return rgbColor(f*c.r, f*c.g, f*c.b);
}

ostream& operator<<(ostream& out, const color& c);
