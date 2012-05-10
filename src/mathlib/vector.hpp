#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include "sse.hpp"

using namespace std;

class point3;
class vec3;

class vec2 {
    friend class vec3;
    public:
        constexpr vec2() : x_(0), y_(0) {}
        constexpr vec2(const float& x, const float& y) : x_(x), y_(y) {}

        inline constexpr vec2 operator+(const vec2& v) {
            return vec2(x_ + v.x_, y_ + v.y_);
        }

        inline vec2& operator+=(const vec2& v){
            x_ += v.x_;
            y_ += v.y_;
            return (*this);
        }

        inline constexpr vec2 operator-(const vec2& v) {
            return vec2(x_ - v.x_, y_ - v.y_);
        }

        inline vec2& operator-=(const vec2& v){
            return (*this) += -v;
        }

        inline constexpr vec2 operator-() {
            return vec2(-x_, -y_);
        }

        inline constexpr vec2 operator*(const float& f) {
            return vec2(x_ * f, y_ * f);
        }

        inline vec2& operator*=(const float& f){
            x_ *= f;
            y_ *= f;
            return (*this);
        }

        inline constexpr vec2 operator*(const vec2& v) {
            return vec2(x_ * v.x_, y_ * v.y_);
        }

        inline vec2& operator*=(const vec2& v){
            x_ *= v.x_;
            y_ *= v.y_;
            return (*this);
        }

        inline constexpr vec2 operator/(const float& f) {
            return vec2(x_ / f, y_ / f);
        }

        inline vec2& operator/=(const float& f){
            return (*this) *= 1.f / f;
        }

        inline constexpr vec2 operator/(const vec2& v) {
            return vec2(x_ / v.x_, y_ / v.y_);
        }

        inline vec2& operator/=(const vec2& v){
            x_ /= v.x_;
            y_ /= v.y_;
            return (*this);
        }

        inline constexpr bool operator==(const vec2& v) {
            return (x_ == v.x_) && (y_ == v.y_);
        }

        inline constexpr float x() {
            return x_;
        }

        inline float& x() {
            return x_;
        }

        inline constexpr float y() {
            return y_;
        }

        inline float& y() {
            return y_;
        }

    private:
        float x_;
        float y_;
};

class vec3 {
    friend class point3;
    public:
        vec3(const point3& p);

        // Can't use constexpr yet because __m128 is not a literal :C

        /*constexpr*/ vec3() : xyzw{0,0,0,0} {}
        /*constexpr*/ vec3(const float& x, const float& y, const float& z) : xyzw{x, y, z, 0.} {}
        /*constexpr*/ vec3(const vec2& v, const float& f) : xyzw{v.x_, v.y_, f, 0} {}
        /*constexpr*/ vec3(const float& f, const vec2& v) : xyzw{f, v.x_, v.y_, 0} {}
        /*constexpr*/ vec3(const float& f) : xyzw{f,f,f,f} {}
        /*constexpr*/ vec3(const __m128& v) : xyzw(v) {}

        inline const float& operator()(const int& index) const{
#ifdef DEBUG
            assert(index < 3);
#endif
            return ((float*)&xyzw)[index];
        }

        inline float& operator()(const int& index){
#ifdef DEBUG
            assert(index < 3);
#endif
            return ((float*)&xyzw)[index];
        }

        inline const vec3 operator+(const vec3& v) const {
            return vec3(v) += *this;
        }

        inline vec3& operator+=(const vec3& v){
            xyzw = xyzw + v.xyzw;
            return (*this);
        }

        inline const vec3 operator-(const vec3& v) const {
            return vec3(*this) -= v;
        }

        inline vec3& operator-=(const vec3& v){
            return (*this) += -v;
        }

        inline const vec3 operator-() const {
            return vec3(-x(), -y(), -z());
        }

        inline const vec3 operator*(const float& f) const {
            return vec3(*this) *= f;
        }

        inline vec3& operator*=(const float& f){
            xyzw = xyzw * set1ps(f);
            return (*this);
        }

        inline const vec3 operator*(const vec3& v) const {
            return vec3(xyzw * v.xyzw);
        }

        inline vec3& operator*=(const vec3& v){
            xyzw = xyzw * v.xyzw;
            return (*this);
        }

        inline const vec3 operator/(const float& f) const {
            return vec3(xyzw / set1ps(f));
        }

        inline vec3& operator/=(const float& f){
            xyzw = xyzw / set1ps(f);
            return (*this);
        }

        inline const vec3 operator/(const vec3& v) const {
            return vec3(xyzw / v.xyzw);
        }

        inline vec3& operator/=(const vec3& v){
            xyzw = xyzw / v.xyzw;
            return (*this);
        }

        inline bool operator==(const vec3& v) const {
            return
                (x() == v.x()) &&
                (y() == v.y()) &&
                (z() == v.z());
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

class vec4 {
    public:
        // Can't use constexpr yet because __m128 is not a literal :C

        /*constexpr*/ vec4() : xyzw{0,0,0,0} {}
        /*constexpr*/ vec4(const float& x, const float& y, const float& z, const float& w) : xyzw{x,y,z,w} {}
        /*constexpr*/ vec4(const float& f) : xyzw{f,f,f,f} {}
        /*constexpr*/ vec4(const __m128& v) : xyzw(v) {}
        vec4(const vec3& v, const float& w) : xyzw{v.x(), v.y(), v.z(), w} {}

        inline const float& operator()(const int& index) const{
#ifdef DEBUG
            assert(index < 4);
#endif
            return ((float*)&xyzw)[index];
        }

        inline float& operator()(const int& index){
#ifdef DEBUG
            assert(index < 4);
#endif
            return ((float*)&xyzw)[index];
        }

        inline const vec4 operator+(const vec4& v) const {
            return vec4(*this) += v;
        }

        inline vec4& operator+=(const vec4& v){
            xyzw = xyzw + v.xyzw;
            return (*this);
        }

        inline const vec4 operator-(const vec4& v) const {
            return vec4(xyzw - v.xyzw);
        }

        inline vec4& operator-=(const vec4& v){
            xyzw = xyzw - v.xyzw;
            return (*this);
        }

        inline const vec4 operator-() const {
            return vec4(zerops() - xyzw);
        }

        inline const vec4 operator*(const float& f) const {
            return vec4(*this) *= f;
        }

        inline vec4& operator*=(const float& f){
            xyzw = xyzw * set1ps(f);
            return (*this);
        }

        inline const vec4 operator*(const vec4& v) const {
            return vec4(*this) *= v;
        }

        inline vec4& operator*=(const vec4& v){
            return (*this);
        }

        inline const vec4 operator/(const float& f) const {
            return vec4(*this) *= 1.f / f;
        }

        inline vec4& operator/=(const float& f){
            return (*this) *= 1.f / f;
        }

        inline const vec4 operator/(const vec4& v) const {
            return vec4(*this) /= v;
        }

        inline vec4& operator/=(const vec4& v){
            xyzw = xyzw / v.xyzw;
            return (*this);
        }

        inline bool operator==(const vec4& v) const {
            return
                (x() == v.x()) &&
                (y() == v.y()) &&
                (z() == v.z()) &&
                (w() == v.w());
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

        inline const float& w() const {
            return ((float*)&xyzw)[3];
        }

        inline float& w() {
            return ((float*)&xyzw)[3];
        }

        __m128 xyzw;
};

template <typename vecType>
inline float norm(const vecType& v) {
    return sqrtf(norm2(v));
}

inline float dot(const vec2& u, const vec2& v){
    return
        (u.x() * v.x()) +
        (u.y() * v.y());
}

inline float dot(const vec3& u, const vec3& v){
    return
        (u.x() * v.x()) +
        (u.y() * v.y()) +
        (u.z() * v.z());
}

inline float dot(const vec4& u, const vec4& v){
    return
        (u.x() * v.x()) +
        (u.y() * v.y()) +
        (u.z() * v.z()) +
        (u.w() * v.w());
}

inline vec3 cross(const vec3& a, const vec3& b){
    const __m128 v1 =
            shufps(b.xyzw, b.xyzw, shufarg(0, 1, 0, 2)) *
            shufps(a.xyzw, a.xyzw, shufarg(0, 0, 2, 1));
    const __m128 v2 =
            shufps(b.xyzw, b.xyzw, shufarg(0, 0, 2, 1)) *
            shufps(a.xyzw, a.xyzw, shufarg(0, 1, 0, 2));

    return vec3(v1 - v2);
}

inline const vec2 operator*(const float& f, const vec2& u){
    return u * f;
}

inline const vec3 operator*(const float& f, const vec3& u){
    return u * f;
}

inline const vec4 operator*(const float& f, const vec4& u){
    return u * f;
}

inline vec2& operator*=(const float& f, vec2& u){
    return (u *= f);
}

inline vec3& operator*=(const float& f, vec3& u){
    return (u *= f);
}

inline vec4& operator*=(const float& f, vec4& u){
    return (u *= f);
}

inline const vec2 operator/(const float& f, const vec2& v){
    return vec2(f / v.x(), f / v.y());
}

inline const vec3 operator/(const float& f, const vec3& v){
    return vec3(f / v.x(), f / v.y(), f / v.z());
}

inline const vec4 operator/(const float& f, const vec4& v){
    return vec4(f / v.x(), f / v.y(), f / v.z(), f / v.w());
}

inline const vec3 min(const vec3& a, const vec3& b) {
    return vec3(minps(a.xyzw, b.xyzw));
}

inline const vec3 max(const vec3& a, const vec3& b) {
    return vec3(maxps(a.xyzw, b.xyzw));
}

inline float norm2(const vec2& v) {
    return v.x()*v.x() + v.y()*v.y();
}

inline float norm2(const vec3& v) {
    return v.x()*v.x() + v.y()*v.y() + v.z()*v.z();
}

inline float norm2(const vec4& v) {
    return v.x()*v.x() + v.y()*v.y() + v.z()*v.z() + v.w()*v.w();
}

inline const vec3 normalize(const vec3& u){
    return u / norm(u);
}

ostream& operator<<(ostream& out, const vec2& x);
ostream& operator<<(ostream& out, const vec3& x);
ostream& operator<<(ostream& out, const vec4& x);
