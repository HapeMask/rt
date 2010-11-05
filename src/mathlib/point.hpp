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

        inline const point2 operator+(const vec2& u) const {
            return point2(*this) += u;
        }

        inline point2& operator+=(const vec2& u) {
            x() += u.x();
            y() += u.y();
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
            return vec2(x(), y()) - vec2(p.x(), p.y());
        }

        inline const bool operator==(const point2& p) const {
            return
                (x() == p.x()) &&
                (y() == p.y());
        }

	private:
		float coords[2];
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

        inline const __m128 getSIMD() const {
            return vector;
        }

        inline const point3 operator+(const vec3& u) const {
            return point3(*this) += u;
        }

        inline point3& operator+=(const vec3& u) {
            //x() += u.x();
            //y() += u.y();
            //z() += u.z();
            vector = addps(vector, u.vector);
            return (*this);
        }

        inline const point3 operator-(const vec3& u) const {
            return point3(*this) -= u;
        }

        inline point3& operator-=(const vec3& u) {
            //(*this) += -u;
            vector = subps(vector, u.vector);
            return (*this);
        }

        inline const vec3 operator-(const point3& p) const {
            //return vec3(subps(getSIMD(), p.getSIMD()));
            return vec3(subps(vector, p.vector));
        }

        inline const bool operator==(const point3& p) const {
            return
                (x() == p.x()) &&
                (y() == p.y()) &&
                (z() == p.z());
        }

	private:
        union{
            float coords[4];
            __m128 vector;
        };
};

ostream& operator<<(ostream& out, const point3& p);
