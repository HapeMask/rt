#include "plane.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"

#include <cmath>

plane::plane(const vec3& n, const point3& p0) : normal(n), p0(p0)
{}

bool plane::intersect(ray& r, point3& p){
	const float t = -dot(vec3(r.origin), normal) / dot(normal, r.direction);
	if(t < 0){
		return false;
	}else{
		r.origin += t*r.direction;
		return true;
	}
}
