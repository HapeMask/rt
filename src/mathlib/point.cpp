#include <iostream>

#include "point.hpp"
#include "vector.hpp"

using namespace std;

point2::point2(const vec2& u){
	coords[0] = u(0);
	coords[1] = u(1);
}

const point2 point2::operator+(const vec2& u) const {
	return point2(*this) += u;
}

point2& point2::operator+=(const vec2& u) {
	x() += u.x();
	y() += u.y();
	return (*this);
}

const point2 point2::operator-(const vec2& u) const {
	return point2(*this) -= u;
}

point2& point2::operator-=(const vec2& u) {
	(*this) += -u;
	return (*this);
}

const vec2 point2::operator-(const point2& p) const {
	return vec2(x(), y()) - vec2(p.x(), p.y());
}

const bool point2::operator==(const point2& p) const {
	return
		(x() == p.x()) &&
		(y() == p.y());
}

point3::point3(const vec3& u){
	coords[0] = u(0);
	coords[1] = u(1);
	coords[2] = u(2);
}


const point3 point3::operator+(const vec3& u) const {
	return point3(*this) += u;
}

point3& point3::operator+=(const vec3& u) {
	x() += u.x();
	y() += u.y();
	z() += u.z();
	return (*this);
}

const point3 point3::operator-(const vec3& u) const {
	return point3(*this) -= u;
}

point3& point3::operator-=(const vec3& u) {
	(*this) += -u;
	return (*this);
}

const vec3 point3::operator-(const point3& p) const {
	return vec3(x(), y(), z()) - vec3(p.x(), p.y(), p.z());
}

const bool point3::operator==(const point3& p) const {
	return
		(x() == p.x()) &&
		(y() == p.y()) &&
		(z() == p.z());
}

ostream& operator<<(ostream& out, const point3& p){
	out << "point3(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
	return out;
}
