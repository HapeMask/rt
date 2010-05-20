#ifndef __COLORRT__
#define __COLORRT__

#include <stdint.h>
#include <iostream>
using namespace std;

class color {
	public:
		virtual const float red() const = 0;
		virtual const float green() const = 0;
		virtual const float blue() const = 0;

		const uint8_t R() const{
			return 255.f * red();
		}

		const uint8_t G() const{
			return 255.f * green();
		}

		const uint8_t B() const{
			return 255.f * blue();
		}

		virtual void invert() = 0;
};

class rgbColor : public color {
	public:
        rgbColor(const float& f);
		rgbColor() : r(0), g(0), b(0) {}
		rgbColor(const float& r, const float& g, const float& b);
		rgbColor(const color& c);

		const rgbColor operator*(const float& f) const;
		rgbColor& operator*=(const float& f);
		const rgbColor operator/(const float& f) const;
		rgbColor& operator/=(const float& f);

		inline const rgbColor operator/(const rgbColor& c) const{
            return rgbColor(*this) /= c;
        }

		inline rgbColor& operator/=(const rgbColor& c){
            r /= c.r;
            g /= c.g;
            b /= c.b;
            return (*this);
        }

		const rgbColor operator-(const rgbColor& f) const;
		rgbColor& operator-=(const rgbColor& f);
		const rgbColor operator+(const rgbColor& f) const;
		rgbColor& operator+=(const rgbColor& f);
		const rgbColor operator*(const rgbColor& f) const;
		rgbColor& operator*=(const rgbColor& f);

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

		float r;
		float g;
		float b;
};

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
#endif
