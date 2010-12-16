#pragma once

#include <iostream>
#include <cassert>
#include <string.h>
#ifdef HAVE_SSE2
#include "sse.hpp"
#endif

using namespace std;

class point3;

class vec2 {
	public:
		vec2(){
			values[0] = 0.f;
			values[1] = 0.f;
		}

		vec2(const float& x, const float& y){
			values[0] = x;
			values[1] = y;
		}

		vec2(const vec2& x){
			values[0] = x(0);
			values[1] = x(1);
		}

		const float& operator()(const short& index) const{
#ifdef DEBUG
			assert(index < 2);
#endif
			return values[index];
		}

		float& operator()(const short& index){
#ifdef DEBUG
			assert(index < 2);
#endif
			return values[index];
		}

		const float& x() const {
			return values[0];
		}

		const float& y() const {
			return values[1];
		}

		float& x() {
			return values[0];
		}

		float& y() {
			return values[1];
		}

        inline const vec2 operator+(const vec2& v) const {
            return vec2(*this) += v;
        }

        inline vec2& operator+=(const vec2& v){
            values[0] += v(0);
            values[1] += v(1);
            return (*this);
        }

        inline const vec2 operator-(const vec2& v) const {
            return vec2(*this) -= v;
        }

        inline vec2& operator-=(const vec2& v){
            return (*this) += -v;
        }

        inline const vec2 operator-() const {
            return vec2(-x(), -y());
        }

        inline const vec2 operator*(const float& x) const {
            return vec2(*this) *= x;
        }

        inline vec2& operator*=(const float& x){
            values[0] *= x;
            values[1] *= x;
            return (*this);
        }

        inline const vec2 operator*(const vec2& v) const {
            return vec2(*this) *= v;
        }

        inline vec2& operator*=(const vec2& v){
            values[0] *= v(0);
            values[1] *= v(1);
            return (*this);
        }

        inline const vec2 operator/(const float& x) const {
            return vec2(*this) *= 1.f / x;
        }

        inline vec2& operator/=(const float& x){
            return (*this) *= 1.f / x;
        }

        inline const vec2 operator/(const vec2& v) const {
            return vec2(*this) /= v;
        }

        inline vec2& operator/=(const vec2& v){
            values[0] /= v(0);
            values[1] /= v(1);
            return (*this);
        }

        inline const bool operator==(const vec2& v) const {
            return (x() == v.x()) && (y() == v.y());
        }

        inline const float length() const {
            return sqrtf(length2());
        }

        inline const float length2() const {
            return values[0]*values[0] + values[1]*values[1];
        }

	private:
		float values[2];
};

class vec3 {
    friend class point3;
	public:
		vec3(){
            values[0] = 0.f;
            values[1] = 0.f;
            values[2] = 0.f;
            values[3] = 0.f;
		}

		vec3(const float& x, const float& y, const float& z){
			values[0] = x;
			values[1] = y;
			values[2] = z;
            values[3] = 0;
		}

		vec3(const vec3& x){
#ifdef HAVE_SSE2
			vector = x.vector;
#else
			values[0] = x(0);
			values[1] = x(1);
			values[2] = x(2);
            values[3] = 0.f;
#endif
		}

		vec3(const vec2& u, const float& x){
			values[0] = u(0);
			values[1] = u(1);
			values[2] = x;
            values[3] = 0.f;
		}

		vec3(const float& x, const vec2& u){
			values[0] = x;
			values[1] = u(0);
			values[2] = u(1);
            values[3] = 0.f;
		}

		vec3(const float& x){
			values[0] = x;
			values[1] = x;
			values[2] = x;
            values[3] = 0.f;
		}

#ifdef HAVE_SSE2
        vec3(const __m128 v){
			vector = v;
        }
#endif

		vec3(const point3& p);

		inline const float& operator()(const short& index) const{
#ifdef DEBUG
			assert(index < 3);
#endif
			return values[index];
		}

		inline float& operator()(const short& index){
#ifdef DEBUG
			assert(index < 3);
#endif
			return values[index];
		}

		inline const float& x() const {
			return values[0];
		}

		inline const float& y() const {
			return values[1];
		}

		inline const float& z() const {
			return values[2];
		}

		inline float& x() {
			return values[0];
		}

		inline float& y() {
			return values[1];
		}

		inline float& z() {
			return values[2];
		}

#ifdef HAVE_SSE2
		inline const __m128& getSIMD() const{
            return vector;
		}
#endif

        inline const vec3 operator+(const vec3& v) const {
#ifdef HAVE_SSE2
            return vec3(addps(vector, v.vector));
#else
            return vec3(v) += *this;
#endif
        }

        inline vec3& operator+=(const vec3& v){
#ifdef HAVE_SSE2
            vector = addps(vector, v.vector);
#else
            values[0] += v(0);
            values[1] += v(1);
            values[2] += v(2);
#endif
            return (*this);
        }

        inline const vec3 operator-(const vec3& v) const {
#ifdef HAVE_SSE2
            return vec3(subps(vector, v.vector));
#else
            return vec3(*this) -= v;
#endif
        }

        inline vec3& operator-=(const vec3& v){
#ifdef HAVE_SSE2
            vector = subps(vector, v.vector);
            return (*this);
#else
            return (*this) += -v;
#endif
        }

        inline const vec3 operator-() const {
#ifdef HAVE_SSE2
            return vec3(subps(_mm_setzero_ps(), vector));
#else
            return vec3(-x(), -y(), -z());
#endif
        }

        inline const vec3 operator*(const float& x) const {
#ifdef HAVE_SSE2
            return vec3(mulps(vector, _mm_load1_ps(&x)));
#else
            return vec3(*this) *= x;
#endif
        }

        inline vec3& operator*=(const float& x){
#ifdef HAVE_SSE2
            vector = mulps(vector, _mm_load1_ps(&x));
#else
            values[0] *= x;
            values[1] *= x;
            values[2] *= x;
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
            values[0] *= v(0);
            values[1] *= v(1);
            values[2] *= v(2);
#endif
            return (*this);
        }

        inline const vec3 operator/(const float& x) const {
#ifdef HAVE_SSE2
            return vec3(divps(vector, _mm_load1_ps(&x)));
#else
            return vec3(*this) *= 1.f / x;
#endif
        }

        inline vec3& operator/=(const float& x){
#ifdef HAVE_SSE2
            vector = divps(vector, _mm_load1_ps(&x));
            return (*this);
#else
            return (*this) *= 1.f / x;
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
            values[0] /= v(0);
            values[1] /= v(1);
            values[2] /= v(2);
#endif
            return (*this);
        }

        inline const bool operator==(const vec3& v) const {
            return
                (x() == v.x()) &&
                (y() == v.y()) &&
                (z() == v.z());
        }

        inline const float length() const {
            return sqrtf(length2());
        }

        inline const float length2() const {
            return x()*x() + y()*y() + z()*z();
        }

        inline void normalize() {
            (*this) /= length();
        }

	private:
#ifdef HAVE_SSE2
        union{
            float values[4];
            __m128 vector;
        };
#else
        float values[4];
#endif
};

class vec4 {
	public:
		vec4(){
            memset(values, 0, 16);
		}

		vec4(const float& x, const float& y, const float& z, const float& w){
			values[0] = x;
			values[1] = y;
			values[2] = z;
			values[3] = w;
		}

		vec4(const vec4& x){
			values[0] = x(0);
			values[1] = x(1);
			values[2] = x(2);
			values[3] = x(3);
		}

		vec4(const float& x){
			values[0] = x;
			values[1] = x;
			values[2] = x;
			values[3] = x;
		}

		vec4(const vec3& u, const float& x){
			values[0] = u(0);
			values[1] = u(1);
			values[2] = u(2);
			values[3] = x;
		}

		inline const float& operator()(const short& index) const{
#ifdef DEBUG
			assert(index < 4);
#endif
			return values[index];
		}

		inline float& operator()(const short& index){
#ifdef DEBUG
			assert(index < 4);
#endif
			return values[index];
		}

		inline const float& x() const {
			return values[0];
		}

		inline const float& y() const {
			return values[1];
		}

		inline const float& z() const {
			return values[2];
		}

		inline const float& w() const {
			return values[3];
		}

		inline float& x() {
			return values[0];
		}

		inline float& y() {
			return values[1];
		}

		inline float& z() {
			return values[2];
		}

		inline float& w() {
			return values[3];
		}

#ifdef HAVE_SSE2
        inline const __m128& getSIMD() const{
            return vector;
        }
#endif

        inline const vec4 operator+(const vec4& v) const {
            return vec4(*this) += v;
        }

        inline vec4& operator+=(const vec4& v){
            values[0] += v(0);
            values[1] += v(1);
            values[2] += v(2);
            values[3] += v(3);
            return (*this);
        }

        inline const vec4 operator-(const vec4& v) const {
            return vec4(*this) -= v;
        }

        inline vec4& operator-=(const vec4& v){
            return (*this) += -v;
        }

        inline const vec4 operator-() const {
            return vec4(-x(), -y(), -z(), -w());
        }

        inline const vec4 operator*(const float& x) const {
            return vec4(*this) *= x;
        }

        inline vec4& operator*=(const float& x){
            values[0] *= x;
            values[1] *= x;
            values[2] *= x;
            values[3] *= x;
            return (*this);
        }

        inline const vec4 operator*(const vec4& v) const {
            return vec4(*this) *= v;
        }

        inline vec4& operator*=(const vec4& v){
            return (*this);
        }

        inline const vec4 operator/(const float& x) const {
            return vec4(*this) *= 1.f / x;
        }

        inline vec4& operator/=(const float& x){
            return (*this) *= 1.f / x;
        }

        inline const vec4 operator/(const vec4& v) const {
            return vec4(*this) /= v;
        }

        inline vec4& operator/=(const vec4& v){
            values[0] /= v(0);
            values[1] /= v(1);
            values[2] /= v(2);
            values[3] /= v(3);
            return (*this);
        }

        inline const bool operator==(const vec4& v) const {
            return
                (x() == v.x()) &&
                (y() == v.y()) &&
                (z() == v.z()) &&
                (w() == v.w());
        }

        inline const float length() const {
            return sqrtf(length2());
        }

        inline const float length2() const {
            return values[0]*values[0] +
                values[1]*values[1] +
                values[2]*values[2] +
                values[3]*values[3];
        }

	private:
#ifdef HAVE_SSE2
        union{
            float values[4];
            __m128 vector;
        };
#else
        float values[4];
#endif
};

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

inline vec3 cross(const vec3& u, const vec3& v){
	return vec3(
			(u(1) * v(2)) - (u(2) * v(1)),
			(u(2) * v(0)) - (u(0) * v(2)),
			(u(0) * v(1)) - (u(1) * v(0))
		);
}

inline const vec2 normalize(const vec2& u){
	return u / u.length();
}

inline const vec3 normalize(const vec3& u){
	return u / u.length();
}

inline const vec4 normalize(const vec4& u){
	return u / u.length();
}

inline const vec3 operator*(const float& x, const vec3& u){
	return u * x;
}

inline const vec2 operator*(const float& x, const vec2& u){
	return u * x;
}

inline vec3& operator*=(const float& x, vec3& u){
	return (u *= x);
}

inline const vec3 operator/(const float& x, const vec3& v){
	return vec3(x / v(0), x / v(1), x / v(2));
}

ostream& operator<<(ostream& out, const vec2& x);
ostream& operator<<(ostream& out, const vec3& x);
ostream& operator<<(ostream& out, const vec4& x);
