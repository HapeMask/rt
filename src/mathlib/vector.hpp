#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include "sse.hpp"

using namespace std;

class point3;
//class vec3;

class vec2 {
    //friend class vec3;
    public:
        constexpr vec2() : x(0), y(0) {}
        constexpr vec2(const float& x, const float& y) : x(x), y(y) {}

        inline constexpr vec2 operator+(const vec2& v) {
            return vec2(x + v.x, y + v.y);
        }

        inline vec2& operator+=(const vec2& v){
            x += v.x;
            y += v.y;
            return (*this);
        }

        inline constexpr vec2 operator-(const vec2& v) {
            return vec2(x - v.x, y - v.y);
        }

        inline vec2& operator-=(const vec2& v){
            return (*this) += -v;
        }

        inline constexpr vec2 operator-() {
            return vec2(-x, -y);
        }

        inline constexpr vec2 operator*(const float& f) {
            return vec2(x * f, y * f);
        }

        inline vec2& operator*=(const float& f){
            x *= f;
            y *= f;
            return (*this);
        }

        inline constexpr vec2 operator*(const vec2& v) {
            return vec2(x * v.x, y * v.y);
        }

        inline vec2& operator*=(const vec2& v){
            x *= v.x;
            y *= v.y;
            return (*this);
        }

        inline constexpr vec2 operator/(const float& f) {
            return vec2(x / f, y / f);
        }

        inline vec2& operator/=(const float& f){
            return (*this) *= 1.f / f;
        }

        inline constexpr vec2 operator/(const vec2& v) {
            return vec2(x / v.x, y / v.y);
        }

        inline vec2& operator/=(const vec2& v){
            x /= v.x;
            y /= v.y;
            return (*this);
        }

        inline constexpr bool operator==(const vec2& v) {
            return (x == v.x) && (y == v.y);
        }

        float x;
        float y;
};

class vec3 {
    friend class point3;
    public:
        vec3(const point3& p);

        vec3() : xyzw{0,0,0,0} {}
        vec3(const float& x, const float& y, const float& z) : xyzw{x, y, z, 0.f} {}
        vec3(const vec2& v, const float& f) : xyzw{v.x, v.y, f, 0} {}
        vec3(const float& f, const vec2& v) : xyzw{f, v.x, v.y, 0} {}
        vec3(const float& f) : xyzw{f,f,f,f} {}
        vec3(const __m128& v) : xyzw(v) {}

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
            return vec3(-x, -y, -z);
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
                (x == v.x) &&
                (y == v.y) &&
                (z == v.z);
        }

        union {
            struct { float x, y, z, w; };
            __m128 xyzw;
        };

    protected:
        vec3(const float& x, const float& y, const float& z, const float& w) : xyzw{x,y,z,w} {}
};

class vec4 : public vec3 {
    public:
        vec4() : vec3() {}
        vec4(const float& x, const float& y, const float& z, const float& w) : vec3(x,y,z,w) {}
        vec4(const float& f) : vec3(f) {}
        vec4(const __m128& v) : vec3(v) {}
        vec4(const vec3& v, const float& w) : vec3(v.x, v.y, v.z, w) {}

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

        inline bool operator==(const vec4& v) const {
            return
                (x == v.x) &&
                (y == v.y) &&
                (z == v.z) &&
                (w == v.w);
        }
};

inline float dot(const vec2& u, const vec2& v){
    return
        (u.x * v.x) +
        (u.y * v.y);
}

inline float dot(const vec3& u, const vec3& v){
#ifdef __SSE4_1__
    return _mm_cvtss_f32(dpps(u, v, DOTMASK_3));
#else
    return
        (u.x * v.x) +
        (u.y * v.y) +
        (u.z * v.z);
#endif
}

inline float dot(const vec4& u, const vec4& v){
#ifdef __SSE4_1__
    return _mm_cvtss_f32(dpps(u, v, DOTMASK_4));
#else
    return
        (u.x * v.x) +
        (u.y * v.y) +
        (u.z * v.z) +
        (u.w * v.w);
#endif
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

inline vec2& operator*=(const float& f, vec2& u){
    return (u *= f);
}

inline vec3& operator*=(const float& f, vec3& u){
    return (u *= f);
}

inline const vec2 operator/(const float& f, const vec2& v){
    return vec2(f / v.x, f / v.y);
}

inline const vec3 operator/(const float& f, const vec3& v){
    return vec3(f / v.x, f / v.y, f / v.z);
}

inline const vec4 operator/(const float& f, const vec4& v){
    return vec4(f / v.x, f / v.y, f / v.z, f / v.w);
}

inline const vec3 min(const vec3& a, const vec3& b) {
    return vec3(minps(a.xyzw, b.xyzw));
}

inline const vec3 max(const vec3& a, const vec3& b) {
    return vec3(maxps(a.xyzw, b.xyzw));
}

inline float norm2(const vec2& v) {
    return v.x*v.x + v.y*v.y;
}

inline float norm(const vec2& v) {
    return sqrtf(norm2(v));
}

inline float norm(const vec3& v) {
#ifdef __SSE4_1__
#error "TEST ME"
    return _mm_cvtss_f32(sqrtss(dpps(v, v, DOTMASK_3)));
#else
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
#endif
}

inline float norm(const vec4& v) {
#ifdef __SSE4_1__
#error "TEST ME"
    return _mm_cvtss_f32(sqrtss(dpps(v, v, DOTMASK_4)));
#else
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
#endif
}

inline float norm2(const vec3& v) {
#ifdef __SSE4_1__
#error "TEST ME"
    return _mm_cvtss_f32(dpps(v, v, DOTMASK_3));
#else
    return v.x*v.x + v.y*v.y + v.z*v.z;
#endif
}

inline float norm2(const vec4& v) {
#ifdef __SSE4_1__
#error "TEST ME"
    return _mm_cvtss_f32(dpps(v, v, DOTMASK_4));
#else
    return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
#endif
}

inline const vec3 normalize(const vec3& u){
#ifdef __SSE4_1__
#error "TEST ME"
    return u.xyzw * rsqrtps(dpps(u.xyzw, u.xyzw, 0x77));
    // More Accurate:
    //return u.xyzw / sqrtps(dpps(u.xyzw, u.xyzw, 0x7F));
#else
    return u / norm(u);
#endif
}

inline const vec3 normalize(const vec4& u){
#ifdef __SSE4_1__
#error "TEST ME"
    return u.xyzw * rsqrtps(dpps(u.xyzw, u.xyzw, 0xFF));
    // More Accurate:
    //return u.xyzw / sqrtps(dpps(u.xyzw, u.xyzw, 0xFF));
#else
    return u / norm(u);
#endif
}

ostream& operator<<(ostream& out, const vec2& x);
ostream& operator<<(ostream& out, const vec3& x);
ostream& operator<<(ostream& out, const vec4& x);
