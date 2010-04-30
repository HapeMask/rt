#ifndef __RT_POINT__
#define __RT_POINT__

#include <iostream>
#include <cassert>
#include "sse.hpp"

using namespace std;

class vec3;
class vec2;

class point2 {
	public:
		point2(const vec2& u);

		point2(){
			coords[0] = 0.f;
			coords[1] = 0.f;
		}

		point2(const float& x, const float& y){
			coords[0] = x;
			coords[1] = y;
		}

		point2(const point2& p){
			coords[0] = p(0);
			coords[1] = p(1);
		}

		inline const float& operator()(const unsigned short& index) const{
#ifdef DEBUG
			assert(index < 2);
#endif
			return coords[index];
		}

		inline float& operator()(const unsigned short& index){
#ifdef DEBUG
			assert(index < 2);
#endif
			return coords[index];
		}

		inline const float& x() const {
			return coords[0];
		}

		inline const float& y() const {
			return coords[1];
		}

		inline float& x() {
			return coords[0];
		}

		inline float& y() {
			return coords[1];
		}

		const point2 operator+(const vec2& u) const;
		point2& operator+=(const vec2& u);

		const point2 operator-(const vec2& u) const;
		point2& operator-=(const vec2& u);
		const vec2 operator-(const point2& p) const;

		const bool operator==(const point2& p) const;

	private:
		float coords[2];
};

class point3 {
	public:
		point3(const vec3& u);

		point3(){
			coords[0] = 0.f;
			coords[1] = 0.f;
			coords[2] = 0.f;
		}

		point3(const float& x, const float& y, const float& z){
			coords[0] = x;
			coords[1] = y;
			coords[2] = z;
		}

		point3(const point3& p){
			coords[0] = p(0);
			coords[1] = p(1);
			coords[2] = p(2);
		}

		inline const float& operator()(const unsigned short& index) const{
#ifdef DEBUG
			assert(index < 3);
#endif
			return coords[index];
		}

		inline float& operator()(const unsigned short& index){
#ifdef DEBUG
			assert(index < 3);
#endif
			return coords[index];
		}

		inline const float& x() const {
			return coords[0];
		}

		inline const float& y() const {
			return coords[1];
		}

		inline const float& z() const {
			return coords[2];
		}

		inline float& x() {
			return coords[0];
		}

		inline float& y() {
			return coords[1];
		}

		inline float& z() {
			return coords[2];
		}

        inline __m128 getSIMD(){
            return simdCoords;
        }

        inline const __m128 getSIMD() const {
            return simdCoords;
        }

		const point3 operator+(const vec3& u) const;
		point3& operator+=(const vec3& u);

		const point3 operator-(const vec3& u) const;
		point3& operator-=(const vec3& u);
		const vec3 operator-(const point3& p) const;

		const bool operator==(const point3& p) const;

	private:
        union{
            float coords[4] ALIGN_16;
            __m128 simdCoords;
        };
};

ostream& operator<<(ostream& out, const point3& p);
#endif
