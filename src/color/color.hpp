#pragma once

#ifdef RT_USE_QT
#include <QColor>
#include <QColormap>
#endif

#include <cmath>
#include <stdint.h>
#include <iostream>
using std::ostream;

#include "mathlib/sse.hpp"
#include "mathlib/vector.hpp"

class color {
	public:
		float red() const { return 0; }
		float green() const { return 0; }
		float blue() const { return 0; }
		float alpha() const { return 0; }

        inline uint8_t R() const { return 0; }
        inline uint8_t G() const { return 0; }
        inline uint8_t B() const { return 0; }
        inline uint8_t A() const { return 0; }

        inline float gray() const {
            return red() * 0.297f + green() * 0.569f + blue() * 0.114f;
        }

        inline float avg() const {
            return (red() + blue() + green()) / 3.f;
        }
};

class rgbColor : public color, public vec3 {
	public:
		rgbColor() : vec3(0,0,0,1) {}
		rgbColor(const vec3& v) : vec3(v) {}
        rgbColor(const float& r, const float& g, const float& b) :
            vec3(r,g,b,1) {}
        rgbColor(const float& r, const float& g, const float& b, const float& a) :
            vec3(r,g,b,a) {}
        rgbColor(const int& r, const int& g, const int& b) :
            vec3(r/255.f, g/255.f, b/255.f, 1.f) {}
        rgbColor(const int& r, const int& g, const int& b, const int& a) :
            vec3(r/255.f, g/255.f, b/255.f, a/255.f) {}
        rgbColor(const __m128& m) : vec3(m) {}

        explicit rgbColor(const float& f): vec3(f,f,f,1) {}

        inline rgbColor inverse() const {
            return rgbColor(1.f) - (*this);
        }

        const float& red() const { return x; }
        const float& green() const { return y; }
        const float& blue() const { return z; }
        const float& alpha() const { return w; }

        inline uint8_t R() const { return 255.f * red(); }
        inline uint8_t G() const { return 255.f * green(); }
        inline uint8_t B() const { return 255.f * blue(); }
        inline uint8_t A() const { return 255.f * alpha(); }

        inline bool isBlack() const {
            return (red() <= 0 && green() <= 0 && blue() <= 0);
        }

#ifdef RT_USE_QT
        inline const QColor qcolor() const {
            return QColor(255.f * x, 255.f * y, 255.f * z);
        }

        inline uint toUint() const {
            return QColormap::instance().pixel(qcolor());
        }
#endif
};

inline const rgbColor lerp(const rgbColor& a, const rgbColor& b, const float& alpha) {
    return (1.f - alpha) * a + alpha * b;
}

ostream& operator<<(ostream& out, const color& c);
