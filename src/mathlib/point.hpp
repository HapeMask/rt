#pragma once

#include <iostream>
#include <cassert>
#include "sse.hpp"
#include "vector.hpp" 

using namespace std;

class point2 {
	public:
		point2(const vec2& u);

        constexpr point2() : x_(0), y_(0) {}
        constexpr point2(const float& x, const float& y) : x_(x), y_(y) {}
        constexpr point2(const point2& p) : x_(p.x_), y_(p.y_) {}

        inline const point2 operator+(const vec2& u) const {
            return point2(*this) += u;
        }

        inline point2& operator+=(const vec2& u) {
            x_ += u.x();
            y_ += u.y();
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
            return vec2(x_, y_) - vec2(p.x_, p.y_);
        }

        inline bool operator==(const point2& p) const {
            return
                (x_ == p.x_) &&
                (y_ == p.y_);
        }

        float x_;
        float y_;
};

class point3 {
	friend class vec3;
	public:
		point3(const vec3& u);

        point3() : xyzw(zerops()) {}

        point3(const float& x_, const float& y_, const float& z_) {
            x() = x_;
            y() = y_;
            z() = z_;
        }

        point3(const point3& p) {
            x() = p.x();
            y() = p.y();
            z() = p.z();
        }

        inline const float& operator()(const int& index) const {
#ifdef DEBUG
            assert(index < 3);
#endif
            return *(&x() + index);
        }

        inline float& operator()(const int& index){
#ifdef DEBUG
            assert(index < 3);
#endif
            return *(&x() + index);
        }

        inline const point3 operator+(const vec3& u) const {
            return point3(*this) += u;
        }

        inline point3& operator+=(const vec3& u) {
            xyzw += u.xyzw;
            return (*this);
        }

        inline const point3 operator-(const vec3& u) const {
            return point3(xyzw - u.xyzw);
        }

        inline point3& operator-=(const vec3& u) {
            xyzw -= u.xyzw;
            return (*this);
        }

        inline const vec3 operator-(const point3& p) const {
            return vec3(xyzw - p.xyzw);
        }

        inline bool operator==(const point3& p) const {
            return (x() == p.x()) && (y() == p.y()) && (z() == p.z());
        }

        inline const float& x() const {
            return ((float*)&xyzw)[0];
        }

        inline float& x() {
            return ((float*)&xyzw)[0];
        }

        inline const float& y() const {
            return ((float*)&xyzw)[1];
        }

        inline float& y() {
            return ((float*)&xyzw)[1];
        }

        inline const float& z() const {
            return ((float*)&xyzw)[2];
        }

        inline float& z() {
            return ((float*)&xyzw)[2];
        }

        __m128 xyzw;
};

inline const point3 min(const point3& a, const point3& b) {
    return point3(minps(a.xyzw, b.xyzw));
}

inline const point3 max(const point3& a, const point3& b) {
    return point3(maxps(a.xyzw, b.xyzw));
}

ostream& operator<<(ostream& out, const point3& p);
