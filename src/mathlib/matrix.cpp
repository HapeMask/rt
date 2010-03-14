#include "matrix.hpp"
#include "vector.hpp"

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

const mat3 mat3::invTransform() const{
	return transpose();
}

const mat3 mat3::transpose() const{
	return mat3(hslice(0), hslice(1), hslice(2));
}

const mat4 mat4::operator*(const float& x) const {
	return mat4(*this) *= x;
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

const mat4 mat4::invTransform() const{
	return transpose();
}

const mat4 mat4::transpose() const{
	return mat4(hslice(0), hslice(1), hslice(2), hslice(3));
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
