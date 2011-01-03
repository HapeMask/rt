#pragma once

#ifdef RT_USE_QT
#include <QColor>
#include <QColormap>
#endif

#include <cmath>
#include <stdint.h>
#include <iostream>
using std::ostream;

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
		rgbColor() : r(0), g(0), b(0) {}
		rgbColor(const float& r, const float& g, const float& b);
		rgbColor(const int& r, const int& g, const int& b);
		rgbColor(const color& c);

        explicit rgbColor(const float& f);

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

#ifdef RT_USE_QT
        inline const QColor qcolor() const {
            return QColor(255.f * r, 255.f * g, 255.f * b);
        }

        inline const uint toUint() const {
            return QColormap::instance().pixel(qcolor());
        }
#endif

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

inline const rgbColor lerp(const rgbColor& a, const rgbColor& b, const float& alpha) {
    return (1.f - alpha) * a + alpha * b;
}

ostream& operator<<(ostream& out, const color& c);
