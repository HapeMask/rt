#ifndef __MAT__
#define __MAT__

#include <iostream>
#include "vector.hpp"
#include "point.hpp"

using namespace std;

class mat3 {
	public:
		/**
		 * Default constructor = identity.
		 */
		mat3(){
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					values[i][j] = (i==j) ? 1.f : 0.f;
				}
			}
		}

		mat3(const mat3& m){
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					values[i][j] = m(i,j);
				}
			}
		}

		/**
		 * Column-vector constructor.
		 */
		mat3(const vec3& a, const vec3& b, const vec3& c){
			for(int i=0; i<3; i++){
				values[i][0] = a(i);
				values[i][1] = b(i);
				values[i][2] = c(i);
			}
		}

		mat3(float* v){
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					values[i][j] = v[3*i+j];
				}
			}
		}

		const float& operator()(const int& i, const int& j) const {
#ifdef DEBUG
			assert(i >= 0 && i < 3 && j >= 0 && j < 3);
#endif
			return values[i][j];
		}

		float& operator()(const int& i, const int& j){
#ifdef DEBUG
			assert(i >= 0 && i < 3 && j >= 0 && j < 3);
#endif
			return values[i][j];
		}

		const mat3 invTransform() const;
		const mat3 transpose() const;

		// Solve Ax = b where A = *this, return x.
		const vec3 solve(const vec3& b);

		const mat3 operator*(const float& x) const;
		mat3& operator*=(const float& x);

		mat3& operator*=(const mat3& m);
		const mat3 operator*(const mat3& m) const;

		const bool operator==(const mat3& m) const;

		const vec3 hslice(const int& i) const {
#ifdef DEBUG
			assert(i<3 && i >= 0);
#endif
			return vec3(values[i][0], values[i][1], values[i][2]);
		}

		const vec3 vslice(const int& i) const {
#ifdef DEBUG
			assert(i<3 && i >= 0);
#endif
			return vec3(values[0][i], values[1][i], values[2][i]);
		}

	private:
		float values[3][3];
};

class mat4 {
	public:
		/**
		 * Default constructor = identity.
		 */
		mat4(){
			for(int i=0; i<4; i++){
				for(int j=0; j<4; j++){
					values[i][j] = (i==j) ? 1.f : 0.f;
				}
			}
		}

		mat4(const mat4& m){
			for(int i=0; i<4; i++){
				for(int j=0; j<4; j++){
					values[i][j] = m(i,j);
				}
			}
		}

		/**
		 * Column-vector constructor.
		 */
		mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d){
			for(int i=0; i<4; i++){
				values[i][0] = a(i);
				values[i][1] = b(i);
				values[i][2] = c(i);
				values[i][3] = d(i);
			}
		}

		mat4(float* v){
			for(int i=0; i<4; i++){
				for(int j=0; j<4; j++){
					values[i][j] = v[4*i+j];
				}
			}
		}

		const float& operator()(const int& i, const int& j) const {
#ifdef DEBUG
			assert(i >= 0 && i < 4 && j >= 0 && j < 4);
#endif
			return values[i][j];
		}

		float& operator()(const int& i, const int& j){
#ifdef DEBUG
			assert(i >= 0 && i < 4 && j >= 0 && j < 4);
#endif
			return values[i][j];
		}

		const mat4 invTransform() const;
		const mat4 transpose() const;

		// Solve Ax = b where A = *this, return x.
		const vec4 solve(const vec4& b);

		const mat4 operator*(const float& x) const;
		mat4& operator*=(const float& x);

		const vec4 operator*(const vec4& u) const;
		mat4& operator*=(const mat4& m);
		const mat4 operator*(const mat4& m) const;

		const bool operator==(const mat4& m) const;

		const vec4 hslice(const int& i) const {
#ifdef DEBUG
			assert(i<4 && i >= 0);
#endif
			return vec4(values[i][0], values[i][1], values[i][2], values[i][3]);
		}

		const vec4 vslice(const int& i) const {
#ifdef DEBUG
			assert(i<4 && i >= 0);
#endif
			return vec4(values[0][i], values[1][i], values[2][i], values[i][3]);
		}

		static mat4 translate(const float& dx, const float& dy, const float& dz);
		static mat4 scale(const float& sx, const float& sy, const float& sz);
		static mat4 lookAt(const point3& pos, const point3& look, const vec3& up);

	private:
		float values[4][4];
};

ostream& operator<<(ostream& out, const mat3& m);

#endif
