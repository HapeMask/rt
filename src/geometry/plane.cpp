#include "plane.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"

#include <cmath>

plane::plane(const vec3& n, const point3& p0, shape* parent) :
	primitive(parent), normal(normalize(n)), p0(p0)
{}

const bool plane::intersect(ray& r) const{
	const float t = -dot(vec3(r.origin), normal) / dot(normal, r.direction);
	if(t < EPSILON){
		return false;
	}else{
		r.origin += t*r.direction;
		return true;
	}
}

const vec3 plane::getNormal(const point3& p) const {
	return normal;
}
