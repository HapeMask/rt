#include "transformation.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "matrix.hpp"
#include "utility.hpp"

#include <cmath>

const point3 transform3d::apply(const point3& p) const {
	const vec4 v = mat * vec4(p.x(), p.y(), p.z(), 1.f);
	return point3(v.x()/v.w(), v.y()/v.w(), v.z()/v.w());
}

const vec3 transform3d::apply(const vec3& v) const {
	return vec3(
			mat(0,0) * v.x() + mat(0,1) * v.y() + mat(0,2) * v.z(),
			mat(1,0) * v.x() + mat(1,1) * v.y() + mat(1,2) * v.z(),
			mat(2,0) * v.x() + mat(2,1) * v.y() + mat(2,2) * v.z()
		);
}

const ray transform3d::apply(const ray& r) const {
	return ray(apply(r.origin), apply(r.direction));
}

const point3 transform3d::unapply(const point3& p) const {
	const vec4 v = inv * vec4(p.x(), p.y(), p.z(), 1.f);
	return point3(v.x()/v.w(), v.y()/v.w(), v.z()/v.w());
}

const vec3 transform3d::unapply(const vec3& v) const {
	return vec3(
			inv(0,0) * v.x() + inv(0,1) * v.y() + inv(0,2) * v.z(),
			inv(1,0) * v.x() + inv(1,1) * v.y() + inv(1,2) * v.z(),
			inv(2,0) * v.x() + inv(2,1) * v.y() + inv(2,2) * v.z()
		);
}

const ray transform3d::unapply(const ray& r) const {
    return ray(unapply(r.origin), unapply(r.direction));
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
	const vec3 dir(normalize(look - pos));
	const vec3 right(cross(dir, normalize(up)));
	const vec3 newUp(cross(right, dir));

    const float v[4][4] = {
        {right.x(), newUp.x(), dir.x(), pos.x()},
        {right.y(), newUp.y(), dir.y(), pos.y()},
        {right.z(), newUp.z(), dir.z(), pos.z()},
        {0.f, 0.f, 0.f, 1.f}
    };

    return transform3d(mat4(v).inverse());
}

const transform3d perspective(const float& fov, const float& near, const float& far){
	const float invTan = 1.f / tan(radians(fov) / 2.f);
	const float v[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, far / (far - near), -(far*near)/(far - near)},
		{0, 0, 1, 0}
	};

	return scale(invTan, invTan, 1.f) * transform3d(mat4(v));
}
