#include "sphere.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"

#include <cmath>

sphere::sphere(const point3& p, const float& r, shape* parent) :
	primitive(parent), location(p), radius(r), radius2(r*r)
{}

const bool sphere::intersect(ray& r) const {
	vec3 dir = r.origin - location;
	const float A = dot(r.direction, r.direction);
	const float B = dot(2.f*dir, r.direction);
	const float C = dot(dir, dir) - (radius*radius);
	const float s = (B*B - 4*A*C);
	if(s < EPSILON){
		return false;
	}

	float q;
	if(B < EPSILON){
		q = (-B + sqrt(s)) / 2.f;
	}else{
		q = (-B - sqrt(s)) / 2.f;
	}

	const float t0 = q / A;
	const float t1 = C / q;

	if(t0 < EPSILON){
		if(t1 < EPSILON){
			return false;
		}
	}

	r.origin += (min(t0, t1) * r.direction);
	return true;
}

const vec3 sphere::getNormal(const point3& p) const {
	return normalize(p - location);
}
