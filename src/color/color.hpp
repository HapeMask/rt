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
        virtual float red() const = 0;
        virtual float green() const = 0;
        virtual float blue() const = 0;
        virtual float alpha() const = 0;

        virtual uint8_t R() const = 0;
        virtual uint8_t G() const = 0;
        virtual uint8_t B() const = 0;
        virtual uint8_t A() const = 0;

        float gray() const {
            return red() * 0.297f + green() * 0.569f + blue() * 0.114f;
        }

        float avg() const { return (red() + green() + blue()) / 3.f; }
};

class rgbColor : public color, public vec3 {
    public:
        rgbColor() : vec3(0,0,0,1) {}
        rgbColor(const color& c) : vec3(c.red(), c.green(), c.blue(), c.alpha()) {}
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

        rgbColor inverse() const {
            return rgbColor(__m128{1.f,1.f,1.f,1.f} - xyzw);
        }

        float red() const { return x; }
        float green() const { return y; }
        float blue() const { return z; }
        float alpha() const { return w; }

        uint8_t R() const { return 255 * red(); }
        uint8_t G() const { return 255 * green(); }
        uint8_t B() const { return 255 * blue(); }
        uint8_t A() const { return 255 * alpha(); }

        bool isBlack() const {
            return (red() <= EPSILON &&
                    green() <= EPSILON &&
                    blue() <= EPSILON);
        }

#ifdef RT_USE_QT
        QColor qcolor() const { return QColor(R(), G(), B()); }

        uint toUint() const {
            return QColormap::instance().pixel(qcolor());
        }
#endif
};

template <typename Color>
inline Color lerp(const Color& a, const Color& b, const float& alpha) {
    return (1.f - alpha) * a + alpha * b;
}

inline ostream& operator<<(ostream& out, const color& c) {
    out << "color(" << c.red() << ", " << c.green() << ", " << c.blue() << ")";
    return out;
}
