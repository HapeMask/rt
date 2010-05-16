#include <iostream>
#include <cmath>

#include "sse.hpp"
#include "vector.hpp"
#include "point.hpp"

using namespace std;

const vec2 vec2::operator+(const vec2& v) const {
	return vec2(*this) += v;
}

vec2& vec2::operator+=(const vec2& v){
	values[0] += v(0);
	values[1] += v(1);
	return (*this);
}

const vec2 vec2::operator-(const vec2& v) const {
	return vec2(*this) -= v;
}

vec2& vec2::operator-=(const vec2& v){
	return (*this) += -v;
}

const vec2 vec2::operator-() const {
	return vec2(-x(), -y());
}

const vec2 vec2::operator*(const float& x) const {
	return vec2(*this) *= x;
}

vec2& vec2::operator*=(const float& x){
	values[0] *= x;
	values[1] *= x;
	return (*this);
}

const vec2 vec2::operator*(const vec2& v) const {
	return vec2(*this) *= v;
}

vec2& vec2::operator*=(const vec2& v){
	values[0] *= v(0);
	values[1] *= v(1);
	return (*this);
}

const vec2 vec2::operator/(const float& x) const {
	return vec2(*this) *= 1.f / x;
}

vec2& vec2::operator/=(const float& x){
	return (*this) *= 1.f / x;
}

const vec2 vec2::operator/(const vec2& v) const {
	return vec2(*this) /= v;
}

vec2& vec2::operator/=(const vec2& v){
	values[0] /= v(0);
	values[1] /= v(1);
	return (*this);
}

const bool vec2::operator==(const vec2& v) const {
	return (x() == v.x()) && (y() == v.y());
}

const float vec2::length() const {
	return sqrtf(length2());
}

const float vec2::length2() const {
	return dot(*this, *this);
}

vec3::vec3(const point3& p){
	values[0] = p.x();
	values[1] = p.y();
	values[2] = p.z();
}

const vec3 vec3::operator+(const vec3& v) const {
	return vec3(*this) += v;
}

vec3& vec3::operator+=(const vec3& v){
	values[0] += v(0);
	values[1] += v(1);
	values[2] += v(2);
	return (*this);
}

const vec3 vec3::operator-(const vec3& v) const {
	return vec3(*this) -= v;
}

vec3& vec3::operator-=(const vec3& v){
	return (*this) += -v;
}

const vec3 vec3::operator-() const {
	return vec3(-x(), -y(), -z());
}

const vec3 vec3::operator*(const float& x) const {
	return vec3(*this) *= x;
}

vec3& vec3::operator*=(const float& x){
	values[0] *= x;
	values[1] *= x;
	values[2] *= x;
	return (*this);
}

const vec3 vec3::operator*(const vec3& v) const {
	return vec3(*this) *= v;
}

vec3& vec3::operator*=(const vec3& v){
	values[0] *= v(0);
	values[1] *= v(1);
	values[2] *= v(2);
	return (*this);
}

const vec3 vec3::operator/(const float& x) const {
	return vec3(*this) *= 1.f / x;
}

vec3& vec3::operator/=(const float& x){
	return (*this) *= 1.f / x;
}

const vec3 vec3::operator/(const vec3& v) const {
	return vec3(*this) /= v;
}

vec3& vec3::operator/=(const vec3& v){
	values[0] /= v(0);
	values[1] /= v(1);
	values[2] /= v(2);
	return (*this);
}

const bool vec3::operator==(const vec3& v) const {
	return
		(x() == v.x()) &&
		(y() == v.y()) &&
		(z() == v.z());
}

const float vec3::length() const {
	return sqrtf(length2());
}

const float vec3::length2() const {
    return x()*x() + y()*y() + z()*z();
}

const vec4 vec4::operator+(const vec4& v) const {
	return vec4(*this) += v;
}

vec4& vec4::operator+=(const vec4& v){
	values[0] += v(0);
	values[1] += v(1);
	values[2] += v(2);
	values[3] += v(3);
	return (*this);
}

const vec4 vec4::operator-(const vec4& v) const {
	return vec4(*this) -= v;
}

vec4& vec4::operator-=(const vec4& v){
	return (*this) += -v;
}

const vec4 vec4::operator-() const {
	return vec4(-x(), -y(), -z(), -w());
}

const vec4 vec4::operator*(const float& x) const {
	return vec4(*this) *= x;
}

vec4& vec4::operator*=(const float& x){
	values[0] *= x;
	values[1] *= x;
	values[2] *= x;
	values[3] *= x;
	return (*this);
}

const vec4 vec4::operator*(const vec4& v) const {
	return vec4(*this) *= v;
}

vec4& vec4::operator*=(const vec4& v){
	return (*this);
}

const vec4 vec4::operator/(const float& x) const {
	return vec4(*this) *= 1.f / x;
}

vec4& vec4::operator/=(const float& x){
	return (*this) *= 1.f / x;
}

const vec4 vec4::operator/(const vec4& v) const {
	return vec4(*this) /= v;
}

vec4& vec4::operator/=(const vec4& v){
	values[0] /= v(0);
	values[1] /= v(1);
	values[2] /= v(2);
	values[3] /= v(3);
	return (*this);
}

const bool vec4::operator==(const vec4& v) const {
	return
		(x() == v.x()) &&
		(y() == v.y()) &&
		(z() == v.z()) &&
		(w() == v.w());
}

const float vec4::length() const {
	return sqrtf(length2());
}

const float vec4::length2() const {
	return values[0]*values[0] +
        values[1]*values[1] +
        values[2]*values[2] +
        values[3]*values[3];
}

ostream& operator<<(ostream& out, const vec2& x){
	out << "vec2(" << x(0) << ", " << x(1) << ")";
	return out;
}

ostream& operator<<(ostream& out, const vec3& x){
	out << "vec3(" << x(0) << ", " << x(1) << ", " << x(2) << ")";
	return out;
}

ostream& operator<<(ostream& out, const vec4& x){
	out << "vec4(" << x(0) << ", " << x(1) << ", " << x(2) << ", " << x(3) <<  ")";
	return out;
}
