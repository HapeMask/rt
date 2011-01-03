#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

#ifdef HAVE_SSE2
#include "sse.hpp"
#endif

using namespace std;

class point3;

class vec2 {
    public:
        vec2(){
            x = 0.f;
            y = 0.f;
        }

        vec2(const float& x_, const float& y_){
            x = x_;
            y = y_;
        }

        vec2(const vec2& v){
            x = v.x;
            y = v.y;
        }

        const float& operator()(const int& index) const{
#ifdef DEBUG
            assert(index < 2);
#endif
            return *(&x + index);
        }

        float& operator()(const int& index){
#ifdef DEBUG
            assert(index < 2);
#endif
            return *(&x + index);
        }

        inline const vec2 operator+(const vec2& v) const {
            return vec2(*this) += v;
        }

        inline vec2& operator+=(const vec2& v){
            x += v(0);
            y += v(1);
            return (*this);
        }

        inline const vec2 operator-(const vec2& v) const {
            return vec2(*this) -= v;
        }

        inline vec2& operator-=(const vec2& v){
            return (*this) += -v;
        }

        inline const vec2 operator-() const {
            return vec2(-x, -y);
        }

        inline const vec2 operator*(const float& f) const {
            return vec2(*this) *= f;
        }

        inline vec2& operator*=(const float& f){
            x *= f;
            y *= f;
            return (*this);
        }

        inline const vec2 operator*(const vec2& v) const {
            return vec2(*this) *= v;
        }

        inline vec2& operator*=(const vec2& v){
            x *= v(0);
            y *= v(1);
            return (*this);
        }

        inline const vec2 operator/(const float& f) const {
            return vec2(*this) *= 1.f / f;
        }

        inline vec2& operator/=(const float& f){
            return (*this) *= 1.f / f;
        }

        inline const vec2 operator/(const vec2& v) const {
            return vec2(*this) /= v;
        }

        inline vec2& operator/=(const vec2& v){
            x /= v(0);
            y /= v(1);
            return (*this);
        }

        inline const bool operator==(const vec2& v) const {
            return (x == v.x) && (y == v.y);
        }

        union {
            struct {
                float x;
                float y;
            };
            struct {
                float s;
                float t;
            };
        };
};

class vec3 {
    friend class point3;
    public:
        vec3(const point3& p);

        vec3(){
#ifdef HAVE_SSE2
            vector = zerops();
#else
            x = 0.f;
            y = 0.f;
            z = 0.f;
            w = 0.f;
#endif
        }

        vec3(const float& x_, const float& y_, const float& z_){
            x = x_;
            y = y_;
            z = z_;
            w = 0.f;
        }

        vec3(const vec3& v){
#ifdef HAVE_SSE2
            vector = v.vector;
#else
            x = v.x;
            y = v.y;
            z = v.z;
            w = 0.f;
#endif
        }

        vec3(const vec2& v, const float& f){
            x = v.x;
            y = v.y;
            z = f;
            w = 0.f;
        }

        vec3(const float& f, const vec2& v){
            x = f;
            y = v.x;
            z = v.y;
            w = 0.f;
        }

        vec3(const float& f){
#ifdef HAVE_SSE2
            vector = set1ps(f);
            w = 0.f;
#else
            x = f;
            y = f;
            z = f;
            w = 0.f;
#endif
        }

#ifdef HAVE_SSE2
        vec3(const __m128& v) : vector(v) {}
#endif

        inline const float& operator()(const int& index) const{
#ifdef DEBUG
            assert(index < 3);
#endif
            return *(&x + index);
        }

        inline float& operator()(const int& index){
#ifdef DEBUG
            assert(index < 3);
#endif
            return *(&x + index);
        }

        inline const vec3 operator+(const vec3& v) const {
            return vec3(v) += *this;
        }

        inline vec3& operator+=(const vec3& v){
#ifdef HAVE_SSE2
            vector = addps(vector, v.vector);
#else
            x += v.x;
            y += v.y;
            z += v.z;
#endif
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
#ifdef HAVE_SSE2
            vector = mulps(vector, set1ps(f));
#else
            x *= f;
            y *= f;
            z *= f;
#endif
            return (*this);
        }

        inline const vec3 operator*(const vec3& v) const {
#ifdef HAVE_SSE2
            return vec3(mulps(vector, v.vector));
#else
            return vec3(*this) *= v;
#endif
        }

        inline vec3& operator*=(const vec3& v){
#ifdef HAVE_SSE2
            vector = mulps(vector, v.vector);
#else
            x *= v(0);
            y *= v(1);
            z *= v(2);
#endif
            return (*this);
        }

        inline const vec3 operator/(const float& f) const {
#ifdef HAVE_SSE2
            return vec3(divps(vector, set1ps(f)));
#else
            return vec3(*this) *= 1.f / f;
#endif
        }

        inline vec3& operator/=(const float& f){
#ifdef HAVE_SSE2
            vector = divps(vector, set1ps(f));
            return (*this);
#else
            return (*this) *= 1.f / f;
#endif
        }

        inline const vec3 operator/(const vec3& v) const {
#ifdef HAVE_SSE2
            return vec3(divps(vector, v.vector));
#else
            return vec3(*this) /= v;
#endif
        }

        inline vec3& operator/=(const vec3& v){
#ifdef HAVE_SSE2
            vector = divps(vector, v.vector);
#else
            x /= v(0);
            y /= v(1);
            z /= v(2);
#endif
            return (*this);
        }

        inline const bool operator==(const vec3& v) const {
            return
                (x == v.x) &&
                (y == v.y) &&
                (z == v.z);
        }

        union{
#ifdef HAVE_SSE2
            __m128 vector;
#endif
            struct {
                float x;
                float y;
                float z;
                float w;
            };
            struct {
                float s;
                float t;
                float r;
                float q;
            };
        };
};

class vec4 {
    public:
        vec4(){
            x = 0.f;
            y = 0.f;
            z = 0.f;
            w = 0.f;
        }

        vec4(const float& x_, const float& y_, const float& z_, const float& w_){
            x = x_;
            y = y_;
            z = z_;
            w = w_;
        }

        vec4(const vec4& v){
#ifdef HAVE_SSE2
            vector = v.vector;
#else
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
#endif
        }

        vec4(const float& f){
#ifdef HAVE_SSE2
            vector = set1ps(f);
#else
            x = f;
            y = f;
            z = f;
            w = f;
#endif
        }

        vec4(const vec3& v, const float& f){
#ifdef HAVE_SSE2
            vector = v.vector;
#else
            x = v.x;
            y = v.y;
            z = v.z;
#endif
            w = f;
        }

#ifdef HAVE_SSE2
        vec4(const __m128& v) : vector(v) {}
#endif

        inline const float& operator()(const int& index) const{
#ifdef DEBUG
            assert(index < 4);
#endif
            return *(&x + index);
        }

        inline float& operator()(const int& index){
#ifdef DEBUG
            assert(index < 4);
#endif
            return *(&x + index);
        }

        inline const vec4 operator+(const vec4& v) const {
            return vec4(*this) += v;
        }

        inline vec4& operator+=(const vec4& v){
#ifdef HAVE_SSE2
            vector = addps(vector, v.vector);
#else
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
#endif
            return (*this);
        }

        inline const vec4 operator-(const vec4& v) const {
#ifdef HAVE_SSE2
            return vec4(subps(vector, v.vector));
#else
            return vec4(*this) -= v;
#endif
        }

        inline vec4& operator-=(const vec4& v){
#ifdef HAVE_SSE2
            vector = subps(vector, v.vector);
            return (*this);
#else
            return (*this) += -v;
#endif
        }

        inline const vec4 operator-() const {
            return vec4(-x, -y, -z, -w);
        }

        inline const vec4 operator*(const float& f) const {
            return vec4(*this) *= f;
        }

        inline vec4& operator*=(const float& f){
#ifdef HAVE_SSE2
            vector = mulps(vector, set1ps(f));
#else
            x *= f;
            y *= f;
            z *= f;
            w *= f;
#endif
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
#ifdef HAVE_SSE
            vector = divps(vector, v.vector);
#else
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
#endif
            return (*this);
        }

        inline const bool operator==(const vec4& v) const {
            return
                (x == v.x) &&
                (y == v.y) &&
                (z == v.z) &&
                (w == v.w);
        }

        union{
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

template <typename vecType>
inline const float norm(const vecType& v) {
    return sqrtf(norm2(v));
}

inline float dot(const vec2& u, const vec2& v){
    return
        (u.x * v.x) +
        (u.y * v.y);
}

inline float dot(const vec3& u, const vec3& v){
    return
        (u.x * v.x) +
        (u.y * v.y) +
        (u.z * v.z);
}

inline float dot(const vec4& u, const vec4& v){
    return
        (u.x * v.x) +
        (u.y * v.y) +
        (u.z * v.z) +
        (u.w * v.w);
}

using std::cerr;
using std::endl;
inline vec3 cross(const vec3& a, const vec3& b){
#ifdef HAVE_SSE2
    const __m128 v1 = mulps(
            shufps(b.vector, b.vector, shuffle(0, 1, 0, 2)),
            shufps(a.vector, a.vector, shuffle(0, 0, 2, 1))
        );
    const __m128 v2 = mulps(
            shufps(b.vector, b.vector, shuffle(0, 0, 2, 1)),
            shufps(a.vector, a.vector, shuffle(0, 1, 0, 2))
        );

    return vec3(subps(v1, v2));
#else
    return vec3(
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        );
#endif
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
    return vec2(f / v.x, f / v.y);
}

inline const vec3 operator/(const float& f, const vec3& v){
    return vec3(f / v.x, f / v.y, f / v.z);
}

inline const vec4 operator/(const float& f, const vec4& v){
    return vec4(f / v.x, f / v.y, f / v.z, f / v.w);
}

inline const vec3 min(const vec3& a, const vec3& b) {
#ifdef HAVE_SSE2
    return vec3(minps(a.vector, b.vector));
#else
    return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
#endif
}

inline const vec3 max(const vec3& a, const vec3& b) {
#ifdef HAVE_SSE2
    return vec3(maxps(a.vector, b.vector));
#else
    return vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
#endif
}

inline const float norm2(const vec2& v) {
    return v.x*v.x + v.y*v.y;
}

inline const float norm2(const vec3& v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

inline const float norm2(const vec4& v) {
    return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
}

inline const vec3 normalize(const vec3& u){
    return u / norm(u);
}

ostream& operator<<(ostream& out, const vec2& x);
ostream& operator<<(ostream& out, const vec3& x);
ostream& operator<<(ostream& out, const vec4& x);
