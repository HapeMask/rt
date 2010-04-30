#ifndef __RT_VEC__
#define __RT_VEC__

#include <iostream>
#include <cassert>
#include <string.h>
#include "sse.hpp"

using namespace std;

class point3;
class point2;

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

		const float& operator()(const unsigned short& index) const{
#ifdef DEBUG
			assert(index < 2);
#endif
			return values[index];
		}

		float& operator()(const unsigned short& index){
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

		const vec2 operator+(const vec2& v) const;
		vec2& operator+=(const vec2& v);

		const vec2 operator-(const vec2& v) const;
		vec2& operator-=(const vec2& v);

		const vec2 operator-() const;

		const vec2 operator*(const float& x) const;
		vec2& operator*=(const float& x);
		const vec2 operator*(const vec2& v) const;
		vec2& operator*=(const vec2& v);

		const vec2 operator/(const float& x) const;
		vec2& operator/=(const float& x);
		const vec2 operator/(const vec2& v) const;
		vec2& operator/=(const vec2& v);

		const bool operator==(const vec2& v) const;

		const float length() const;
		const float length2() const;

	private:
		float values[2];
};

class vec3 {
	public:
		vec3(){
            memset(values, 0, 16);
            /*
			values[0] = 0.f;
			values[1] = 0.f;
			values[2] = 0.f;
            */
		}

		vec3(const float& x, const float& y, const float& z){
			values[0] = x;
			values[1] = y;
			values[2] = z;
		}

		vec3(const vec3& x){
			values[0] = x(0);
			values[1] = x(1);
			values[2] = x(2);
		}

		vec3(const vec2& u, const float& x){
			values[0] = u(0);
			values[1] = u(1);
			values[2] = x;
		}

		vec3(const float& x, const vec2& u){
			values[0] = x;
			values[1] = u(0);
			values[2] = u(1);
		}

		vec3(const float& x){
			values[0] = x;
			values[1] = x;
			values[2] = x;
		}

        vec3(const __m128 v){
            simdValues = v;
        }

		vec3(const point3& p);

		const float& operator()(const unsigned short& index) const{
#ifdef DEBUG
			assert(index < 3);
#endif
			return values[index];
		}

		float& operator()(const unsigned short& index){
#ifdef DEBUG
			assert(index < 3);
#endif
			return values[index];
		}

		const float& x() const {
			return values[0];
		}

		const float& y() const {
			return values[1];
		}

		const float& z() const {
			return values[2];
		}

		float& x() {
			return values[0];
		}

		float& y() {
			return values[1];
		}

		float& z() {
			return values[2];
		}

        inline const __m128 getSIMD() const {
            return simdValues;
        }

		const vec3 operator+(const vec3& v) const;
		vec3& operator+=(const vec3& v);
		const vec3 operator-(const vec3& v) const;
		vec3& operator-=(const vec3& v);

		const vec3 operator-() const;

		const vec3 operator*(const float& x) const;
		vec3& operator*=(const float& x);
		const vec3 operator*(const vec3& v) const;
		vec3& operator*=(const vec3& v);

		const vec3 operator/(const float& x) const;
		vec3& operator/=(const float& x);
		const vec3 operator/(const vec3& v) const;
		vec3& operator/=(const vec3& v);

		const bool operator==(const vec3& v) const;

		const float length() const;
		const float length2() const;

	private:
        union{
            float values[4] ALIGN_16;
            __m128 simdValues;
        };
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

		const float& operator()(const unsigned short& index) const{
#ifdef DEBUG
			assert(index < 4);
#endif
			return values[index];
		}

		float& operator()(const unsigned short& index){
#ifdef DEBUG
			assert(index < 4);
#endif
			return values[index];
		}

		const float& x() const {
			return values[0];
		}

		const float& y() const {
			return values[1];
		}

		const float& z() const {
			return values[2];
		}

		const float& w() const {
			return values[3];
		}

		float& x() {
			return values[0];
		}

		float& y() {
			return values[1];
		}

		float& z() {
			return values[2];
		}

		float& w() {
			return values[3];
		}

		const vec4 operator+(const vec4& v) const;
		vec4& operator+=(const vec4& v);
		const vec4 operator-(const vec4& v) const;
		vec4& operator-=(const vec4& v);

		const vec4 operator-() const;

		const vec4 operator*(const float& x) const;
		vec4& operator*=(const float& x);
		const vec4 operator*(const vec4& v) const;
		vec4& operator*=(const vec4& v);

		const vec4 operator/(const float& x) const;
		vec4& operator/=(const float& x);
		const vec4 operator/(const vec4& v) const;
		vec4& operator/=(const vec4& v);

		const bool operator==(const vec4& v) const;

		const float length() const;
		const float length2() const;

	private:
        union{
            float values[4] ALIGN_16;
            __m128 simdValues;
        };
};

float dot(const vec4& u, const vec4& v);
float dot(const vec3& u, const vec3& v);
float dot(const vec2& u, const vec2& v);
vec3 cross(const vec3& u, const vec3& v);

const vec2 normalize(const vec2& u);
const vec3 normalize(const vec3& u);
const vec4 normalize(const vec4& u);

ostream& operator<<(ostream& out, const vec2& x);
ostream& operator<<(ostream& out, const vec3& x);
ostream& operator<<(ostream& out, const vec4& x);

const vec3 operator*(const float& x, const vec3& u);
vec3& operator*=(const float& x, vec3& u);

#endif
