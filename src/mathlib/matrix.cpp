#include "matrix.hpp"
#include "vector.hpp"
#include <iostream>

using std::cerr;
using std::endl;

const mat4 mat4::operator*(const float& x) const {
	return mat4(*this) *= x;
}

const vec4 mat4::operator*(const vec4& u) const{
	return vec4(
			dot(u, hslice(0)),
			dot(u, hslice(1)),
			dot(u, hslice(2)),
			dot(u, hslice(3))
			);
}

mat4& mat4::operator*=(const float& x) {
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			values[i][j] *= x;
		}
	}

	return (*this);
}

mat4& mat4::operator*=(const mat4& m) {
	(*this) = (*this) * m;
	return (*this);
}

const mat4 mat4::operator*(const mat4& m) const{
	mat4 n;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			n(i,j) = dot(hslice(i), m.vslice(j));
		}
	}

	return n;
}

const bool mat4::operator==(const mat4& m) const{
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(values[i][j] != m(i,j)) return false;
		}
	}
	return true;
}

const float mat4::det() const{
	return
		values[0][0]*values[1][1]*values[2][2]*values[3][3] + values[0][0]*values[1][2]*values[2][3]*values[3][1] + values[0][0]*values[1][3]*values[2][1]*values[3][2] +
		values[0][1]*values[1][0]*values[2][3]*values[3][2] + values[0][1]*values[1][2]*values[2][0]*values[3][3] + values[0][1]*values[1][3]*values[2][2]*values[3][0] +
		values[0][2]*values[1][0]*values[2][1]*values[3][3] + values[0][2]*values[1][1]*values[2][3]*values[3][0] + values[0][2]*values[1][3]*values[2][0]*values[3][1] +
		values[0][3]*values[1][0]*values[2][2]*values[3][1] + values[0][3]*values[1][1]*values[2][0]*values[3][2] + values[0][3]*values[1][2]*values[2][1]*values[3][0] -
		values[0][0]*values[1][1]*values[2][3]*values[3][2] - values[0][0]*values[1][2]*values[2][1]*values[3][3] - values[0][0]*values[1][3]*values[2][2]*values[3][1] -
		values[0][1]*values[1][0]*values[2][2]*values[3][3] - values[0][1]*values[1][2]*values[2][3]*values[3][0] - values[0][1]*values[1][3]*values[2][0]*values[3][2] -
		values[0][2]*values[1][0]*values[2][3]*values[3][1] - values[0][2]*values[1][1]*values[2][0]*values[3][3] - values[0][2]*values[1][3]*values[2][1]*values[3][0] -
		values[0][3]*values[1][0]*values[2][1]*values[3][2] - values[0][3]*values[1][1]*values[2][2]*values[3][0] - values[0][3]*values[1][2]*values[2][0]*values[3][1];
}

const mat4 mat4::inverse() const{
	mat4 inv;

	inv(0,0)=   values[1][1]*values[2][2]*values[3][3] - values[1][1]*values[2][3]*values[3][2] - values[2][1]*values[1][2]*values[3][3]
		+ values[2][1]*values[1][3]*values[3][2] + values[3][1]*values[1][2]*values[2][3] - values[3][1]*values[1][3]*values[2][2];
	inv(1,0) =  -values[1][0]*values[2][2]*values[3][3] + values[1][0]*values[2][3]*values[3][2] + values[2][0]*values[1][2]*values[3][3]
		- values[2][0]*values[1][3]*values[3][2] - values[3][0]*values[1][2]*values[2][3] + values[3][0]*values[1][3]*values[2][2];
	inv(2,0) =   values[1][0]*values[2][1]*values[3][3] - values[1][0]*values[2][3]*values[3][1] - values[2][0]*values[1][1]*values[3][3]
		+ values[2][0]*values[1][3]*values[3][1] + values[3][0]*values[1][1]*values[2][3] - values[3][0]*values[1][3]*values[2][1];
	inv(3,0) = -values[1][0]*values[2][1]*values[3][2] + values[1][0]*values[2][2]*values[3][1] + values[2][0]*values[1][1]*values[3][2]
		- values[2][0]*values[1][2]*values[3][1] - values[3][0]*values[1][1]*values[2][2] + values[3][0]*values[1][2]*values[2][1];
	inv(0,1) =  -values[0][1]*values[2][2]*values[3][3] + values[0][1]*values[2][3]*values[3][2] + values[2][1]*values[0][2]*values[3][3]
		- values[2][1]*values[0][3]*values[3][2] - values[3][1]*values[0][2]*values[2][3] + values[3][1]*values[0][3]*values[2][2];
	inv(1,1) =   values[0][0]*values[2][2]*values[3][3] - values[0][0]*values[2][3]*values[3][2] - values[2][0]*values[0][2]*values[3][3]
		+ values[2][0]*values[0][3]*values[3][2] + values[3][0]*values[0][2]*values[2][3] - values[3][0]*values[0][3]*values[2][2];
	inv(2,1) =  -values[0][0]*values[2][1]*values[3][3] + values[0][0]*values[2][3]*values[3][1] + values[2][0]*values[0][1]*values[3][3]
		- values[2][0]*values[0][3]*values[3][1] - values[3][0]*values[0][1]*values[2][3] + values[3][0]*values[0][3]*values[2][1];
	inv(3,1) =  values[0][0]*values[2][1]*values[3][2] - values[0][0]*values[2][2]*values[3][1] - values[2][0]*values[0][1]*values[3][2]
		+ values[2][0]*values[0][2]*values[3][1] + values[3][0]*values[0][1]*values[2][2] - values[3][0]*values[0][2]*values[2][1];
	inv(0,2) =   values[0][1]*values[1][2]*values[3][3] - values[0][1]*values[1][3]*values[3][2] - values[1][1]*values[0][2]*values[3][3]
		+ values[1][1]*values[0][3]*values[3][2] + values[3][1]*values[0][2]*values[1][3] - values[3][1]*values[0][3]*values[1][2];
	inv(1,2) =  -values[0][0]*values[1][2]*values[3][3] + values[0][0]*values[1][3]*values[3][2] + values[1][0]*values[0][2]*values[3][3]
		- values[1][0]*values[0][3]*values[3][2] - values[3][0]*values[0][2]*values[1][3] + values[3][0]*values[0][3]*values[1][2];
	inv(2,2) =  values[0][0]*values[1][1]*values[3][3] - values[0][0]*values[1][3]*values[3][1] - values[1][0]*values[0][1]*values[3][3]
		+ values[1][0]*values[0][3]*values[3][1] + values[3][0]*values[0][1]*values[1][3] - values[3][0]*values[0][3]*values[1][1];
	inv(3,2) = -values[0][0]*values[1][1]*values[3][2] + values[0][0]*values[1][2]*values[3][1] + values[1][0]*values[0][1]*values[3][2]
		- values[1][0]*values[0][2]*values[3][1] - values[3][0]*values[0][1]*values[1][2] + values[3][0]*values[0][2]*values[1][1];
	inv(0,3) =  -values[0][1]*values[1][2]*values[2][3] + values[0][1]*values[1][3]*values[2][2] + values[1][1]*values[0][2]*values[2][3]
		- values[1][1]*values[0][3]*values[2][2] - values[2][1]*values[0][2]*values[1][3] + values[2][1]*values[0][3]*values[1][2];
	inv(1,3) =   values[0][0]*values[1][2]*values[2][3] - values[0][0]*values[1][3]*values[2][2] - values[1][0]*values[0][2]*values[2][3]
		+ values[1][0]*values[0][3]*values[2][2] + values[2][0]*values[0][2]*values[1][3] - values[2][0]*values[0][3]*values[1][2];
	inv(2,3) = -values[0][0]*values[1][1]*values[2][3] + values[0][0]*values[1][3]*values[2][1] + values[1][0]*values[0][1]*values[2][3]
		- values[1][0]*values[0][3]*values[2][1] - values[2][0]*values[0][1]*values[1][3] + values[2][0]*values[0][3]*values[1][1];
	inv(3,3) =  values[0][0]*values[1][1]*values[2][2] - values[0][0]*values[1][2]*values[2][1] - values[1][0]*values[0][1]*values[2][2]
		+ values[1][0]*values[0][2]*values[2][1] + values[2][0]*values[0][1]*values[1][2] - values[2][0]*values[0][2]*values[1][1];

	inv *= 1.f / det();
	return inv;
}

const mat4 mat4::transpose() const{
	return mat4(hslice(0), hslice(1), hslice(2), hslice(3));
}

ostream& operator<<(ostream& out, const mat4& m){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			out << m(i,j);
			if(j < 3)
				out << ", ";
		}
		if(i < 3)
			out << endl;
	}

	return out;
}
