#include "matrix.hpp"
#include "vector.hpp"
#include "point.hpp"

const mat3 mat3::operator*(const float& x) const {
	return mat3(*this) *= x;
}

mat3& mat3::operator*=(const float& x) {
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			values[i][j] *= x;
		}
	}

	return (*this);
}

mat3& mat3::operator*=(const mat3& m) {
	(*this) = (*this) * m;
	return (*this);
}

const mat3 mat3::operator*(const mat3& m) const{
	mat3 n;
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			n(i,j) = dot(hslice(i), m.vslice(j));
		}
	}

	return n;
}

const bool mat3::operator==(const mat3& m) const{
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if(values[i][j] != m(i,j)) return false;
		}
	}
	return true;
}

const mat3 mat3::transpose() const{
	return mat3(hslice(0), hslice(1), hslice(2));
}

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

ostream& operator<<(ostream& out, const mat3& m){
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			out << m(i,j);
			if(j < 2)
				out << ", ";
		}
		if(i < 2)
			out << endl;
	}

	return out;
}

const mat4 mat4::translate(const float& dx, const float& dy, const float& dz){
	mat4 m;
	m(0,3) = dx;
	m(1,3) = dy;
	m(2,3) = dz;

	return m;
}

const mat4 mat4::scale(const float& sx, const float& sy, const float& sz){
	mat4 m;
	m(0,0) = sx;
	m(1,1) = sy;
	m(2,2) = sz;

	return m;
}

const mat4 mat4::lookAt(const point3& pos, const point3& look, const vec3& up){
	vec3 dir = normalize(vec3(look) - vec3(pos));
	vec3 right = cross(dir, normalize(up));
	vec3 newUp = cross(right, dir);

	mat4 m;
	m(0,0) = right.x();
	m(1,0) = right.y();
	m(2,0) = right.z();
	m(3,0) = 0.f;

	m(0,1) = newUp.x();
	m(1,1) = newUp.y();
	m(2,1) = newUp.z();
	m(3,1) = 0.f;

	m(0,2) = dir.x();
	m(1,2) = dir.y();
	m(2,2) = dir.z();
	m(3,2) = 0.f;

	m(0,3) = pos.x();
	m(1,3) = pos.y();
	m(2,3) = pos.z();
	m(3,3) = 1.f;

	return m.inverse();
}
