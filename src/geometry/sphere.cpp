#include "sphere.hpp"
#include "shape.hpp"
#include "primitive.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"

#include "utility.hpp"

#include <cmath>

sphere::sphere(const point3& p, const float& r) :
	primitive(aabb(
                    vec3(p.x()-r, p.y()-r, p.z()-r),
                    vec3(p.x()+r, p.y()+r, p.z()+r)
                )
            ),
	location(p), radius(r), radius2(r*r)
{}

const intersection sphere::intersect(ray& r) const {
	const vec3 dir(r.origin - location);
	const float A = dot(r.direction, r.direction);
	const float B = dot(2.f*dir, r.direction);
	const float C = dot(dir, dir) - (radius*radius);
	const float s = (B*B - 4*A*C);

	if(s < 0.f){
		return noIntersect;
	}

    const float q = (B < EPSILON) ? (-B + sqrt(s)) / 2.f : (-B - sqrt(s)) / 2.f;;
	const float t0 = q / A;
	const float t1 = C / q;

	if( (t0 <= r.tMin || t0 >= r.tMax) &&
		(t1 <= r.tMin || t1 >= r.tMax)){
        return noIntersect;
    }

    float t = 0.f;
    if(t0 >= r.tMin && t0 <= r.tMax && t0 > 0){
        if(t1 >= r.tMin && t1 <= r.tMax && t1 > 0){
            t = min(t0, t1);
        }else{
            t = t0;
        }
    }else{
        t = t1;
    }

	r.origin += t * r.direction;
    intersection isect(parent, this, t);
    isect.normal = normalize(r.origin - location);
    makeCoordinateSystem(isect.normal, isect.dpdu, isect.dpdv);
    makeCoordinateSystem(isect.normal, isect.dsdu, isect.dsdv);
    isect.shadingNormal = isect.normal;
	return isect;
}

const vec3 sphere::getNormal(const point3& p) const{
    return normalize(p - location);
}

const point3 sphere::sampleSurface(const float& u0, const float& u1) const{
    return point3(0,0,0);
}
