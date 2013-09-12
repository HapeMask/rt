#pragma once

#include <iostream>
#include <cassert>
#include "sse.hpp"
#include "vector.hpp" 

using namespace std;

class point2 {
	public:
		point2(const vec2& u);

        constexpr point2() : x(0), y(0) {}
        constexpr point2(const float& x, const float& y) : x(x), y(y) {}
        constexpr point2(const point2& p) : x(p.x), y(p.y) {}

        inline const point2 operator+(const vec2& u) const {
            return point2(*this) += u;
        }

        inline point2& operator+=(const vec2& u) {
            x += u.x;
            y += u.y;
            return (*this);
        }

        inline const point2 operator-(const vec2& u) const {
            return point2(*this) -= u;
        }

        inline point2& operator-=(const vec2& u) {
            (*this) += -u;
            return (*this);
        }

        inline const vec2 operator-(const point2& p) const {
            return vec2(x, y) - vec2(p.x, p.y);
        }

        inline bool operator==(const point2& p) const {
            return
                (x == p.x) &&
                (y == p.y);
        }

        float x;
        float y;
};

class point3 : public vec3 {
	public:
        point3() : vec3() {}
		point3(const vec3& u) : vec3(u) {}
        point3(const float& x, const float& y, const float& z) : vec3(x, y, z) {}

		explicit point3(const float& f) : vec3(f) {}
};

inline const point3 min(const point3& a, const point3& b) {
    return point3(minps(a.xyzw, b.xyzw));
}

inline const point3 max(const point3& a, const point3& b) {
    return point3(maxps(a.xyzw, b.xyzw));
}

ostream& operator<<(ostream& out, const point3& p);
