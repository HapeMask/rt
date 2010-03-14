#include "matrix.hpp"
#include "vector.hpp"

const float mat3::det() const {
	return
		values[0][0] * (values[2][2]*values[1][1] - values[2][1]*values[1][2]) -
		values[1][0] * (values[2][2]*values[0][1] - values[2][1]*values[0][2]) +
		values[2][0] * (values[1][2]*values[0][1] - values[1][1]*values[0][2]);
}

/**
 * LOL Lots of ops... probably never use this.
 * Also floating point roundoff errors make it so that
 * m * m.inverse() != IdentityMatrix :-(
 */
const mat3 mat3::inverse() const {
	float d = 1.f / det();
	return mat3(vec3(
				values[2][2]*values[1][1] - values[2][1]*values[1][2],
				-(values[2][2]*values[1][0] - values[2][0]*values[1][2]),
				values[2][1]*values[1][0] - values[2][0]*values[1][1]),
			vec3(
				-(values[2][2]*values[0][1] - values[2][1]*values[0][2]),
				values[2][2]*values[0][0] - values[2][0]*values[0][2],
				-(values[2][1]*values[0][0] - values[2][0]*values[0][1])),
			vec3(
				values[1][2]*values[0][1] - values[1][1]*values[0][2],
				-(values[1][2]*values[0][0] - values[1][0]*values[0][2]),
				values[1][1]*values[0][0] - values[1][0]*values[0][1])) * d;
}

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
