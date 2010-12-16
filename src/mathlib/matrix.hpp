#pragma once

#include <iostream>
#include "vector.hpp"
#include "point.hpp"

using namespace std;

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

		mat4(const float v[4][4]){
			for(int i=0; i<4; i++){
				for(int j=0; j<4; j++){
					values[i][j] = v[i][j];
				}
			}
		}

		const float& operator()(const short& i, const short& j) const {
#ifdef DEBUG
			assert(i < 4 && j < 4);
#endif
			return values[i][j];
		}

		float& operator()(const short& i, const short& j){
#ifdef DEBUG
			assert(i < 4 && j < 4);
#endif
			return values[i][j];
		}

		const mat4 inverse() const;
		const mat4 transpose() const;
		const float det() const;

		// Solve Ax = b where A = *this, return x.
		const vec4 solve(const vec4& b);

		const mat4 operator*(const float& x) const;
		mat4& operator*=(const float& x);

		const vec4 operator*(const vec4& u) const;
		mat4& operator*=(const mat4& m);
		const mat4 operator*(const mat4& m) const;

		const bool operator==(const mat4& m) const;

		const vec4 hslice(const short& i) const {
#ifdef DEBUG
			assert(i<4);
#endif
			return vec4(values[i][0], values[i][1], values[i][2], values[i][3]);
		}

		const vec4 vslice(const short& i) const {
#ifdef DEBUG
			assert(i<4);
#endif
			return vec4(values[0][i], values[1][i], values[2][i], values[3][i]);
		}

	private:
		float values[4][4];
};

ostream& operator<<(ostream& out, const mat4& m);
