#include "sphere.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"

sphere::sphere(const point3& p, const float& r) : location(p), radius(r), radius2(r*r)
{}

bool sphere::intersect(ray& r, point3& p){
	vec3 dir = r.origin - location;
	float b = dot(dir, r.direction);
	float c = dot(dir, dir) - radius2;
	float d = b*b - c;

	if(d > EPSILON){
		return true;
	}else{
		//cerr << "d: " << d << '\n';
		return false;
	}
}
