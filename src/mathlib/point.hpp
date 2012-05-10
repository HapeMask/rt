#pragma once

#include <iostream>
#include <cassert>
#ifdef HAVE_SSE2
#include "sse.hpp"
#endif
#include "vector.hpp" 

using namespace std;

class point2 {
	public:
		point2(const vec2& u);

        constexpr point2() : x(0), y(0) {}
        constexpr point2(const float& x_, const float& y_) : x(x_), y(y_) {}
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

class point3 {
	friend class vec3;
	public:
		point3(const vec3& u);

		point3(){
			coords[0] = 0.f;
			coords[1] = 0.f;
			coords[2] = 0.f;
		}

        point3(const float& x_, const float& y_, const float& z_) : x(x_), y(y_), z(z_) {}
        point3(const point3& p) : x(p.x), y(p.y), z(p.z) {}

		inline const float& operator()(const short& index) const{
#ifdef DEBUG
			assert(index < 3);
#endif
			return coords[index];
		}

		inline float& operator()(const short& index){
#ifdef DEBUG
			assert(index < 3);
#endif
			return coords[index];
		}

        inline const point3 operator+(const vec3& u) const {
            return point3(*this) += u;
        }

        inline point3& operator+=(const vec3& u) {
            x += u.x;
            y += u.y;
            z += u.z;
            return (*this);
        }

        inline const point3 operator-(const vec3& u) const {
            return point3(*this) -= u;
        }

        inline point3& operator-=(const vec3& u) {
            return (*this) += -u;
        }

        inline const vec3 operator-(const point3& p) const {
            return vec3(x - p.x, y - p.y, z - p.z);
        }

        inline bool operator==(const point3& p) const {
            return (x == p.x) && (y == p.y) && (z == p.z);
        }

        union{
            float coords[4];
#ifdef HAVE_SSE2
            __m128 vector;
#endif
            struct {
                float x;
                float y;
                float z;
                float w;
            };
        };
};

inline const point3 min(const point3& a, const point3& b) {
#ifdef HAVE_SSE2
    return point3(minps(a.vector, b.vector));
#else
    return point3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
#endif
}

inline const point3 max(const point3& a, const point3& b) {
#ifdef HAVE_SSE2
    return point3(maxps(a.vector, b.vector));
#else
    return point3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
#endif
}

ostream& operator<<(ostream& out, const point3& p);
