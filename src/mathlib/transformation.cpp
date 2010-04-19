#include "transformation.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "matrix.hpp"
#include "utility.hpp"

#include <cmath>

const point3 transform3d::apply(const point3& p){
	vec4 v = vec4(vec3(p), 1.f);
	v = mat * v;
	return point3(vec3(v.x(), v.y(), v.z()) / v.w());
}

const vec3 transform3d::apply(const vec3& v){
	float x = v.x(), y = v.y(), z = v.z();
	return vec3(
			mat(0,0)*x + mat(0,1)*y + mat(0,2)*z,
			mat(1,0)*x + mat(1,1)*y + mat(1,2)*z,
			mat(2,0)*x + mat(2,1)*y + mat(2,2)*z
		);
}

const ray transform3d::apply(const ray& r){
	ray r2(r);
	r2.origin = apply(r.origin);
	r2.direction = apply(r.direction);
	return r2;
}

const point3 transform3d::reverse(const point3& p){
	vec4 v = vec4(vec3(p), 1.f);
	v = inv * v;
	return point3(vec3(v.x(), v.y(), v.z()) / v.w());
}

const vec3 transform3d::reverse(const vec3& v){
	float x = v.x(), y = v.y(), z = v.z();
	return vec3(
			inv(0,0)*x + inv(0,1)*y + inv(0,2)*z,
			inv(1,0)*x + inv(1,1)*y + inv(1,2)*z,
			inv(2,0)*x + inv(2,1)*y + inv(2,2)*z
		);
}

const ray transform3d::reverse(const ray& r){
	ray r2(r);
	r2.origin = reverse(r.origin);
	r2.direction = reverse(r.direction);
	return r2;
}

const transform3d transform3d::operator*(const transform3d& t) const {
	return transform3d(*this) *= t;
}

transform3d& transform3d::operator*=(const transform3d& t){
	mat *= t.mat;
	inv = mat.inverse();
	return (*this);
}

const transform3d translate(const float& dx, const float& dy, const float& dz){
	mat4 m;
	m(0,3) = dx;
	m(1,3) = dy;
	m(2,3) = dz;

	mat4 i;
	i(0,3) = -dx;
	i(1,3) = -dy;
	i(2,3) = -dz;

	return transform3d(m, i);
}

const transform3d scale(const float& sx, const float& sy, const float& sz){
	mat4 m;
	m(0,0) = sx;
	m(1,1) = sy;
	m(2,2) = sz;

	mat4 i;
	i(0,0) = 1.f/sx;
	i(1,1) = 1.f/sy;
	i(2,2) = 1.f/sz;

	return transform3d(m, i);
}

const transform3d lookAt(const point3& pos, const point3& look, const vec3& up){
	vec3 dir = normalize(look - pos);
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

	return transform3d(m.inverse());
}

const transform3d perspective(const float& fov, const float& near, const float& far){
	const float invTan = 1.f / tan(radians(fov) / 2.f);
	float v[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, far / (far - near), -(far*near)/(far - near)},
		{0, 0, 1, 0}
	};
	mat4 m(v[0]);

	return scale(invTan, invTan, 1.f) * transform3d(m);
}
