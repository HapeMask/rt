#include "light.hpp"
#include "samplers/samplers.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"

areaLight::areaLight(const point3& p, const float& pow, const rgbColor& c,
        const vec3& vA, const vec3& vB) :
    light(p, pow, c), a(vA), b(vB), normal(normalize(cross(vA,vB))),
    A(p - 0.5*vA - 0.5*vB), B(p + 0.5*vA - 0.5*vB), C(p + 0.5*vB - 0.5*vA),
    invArea(1.f/abs(cross(A,B).length()))
{}

const rgbColor areaLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const{
    point3 samplePoint;
    sampleRectangle(samplePoint, a, b, position, u0, u1);
    //uniformSampleRectangle(samplePoint, a, b, position);
    wi = samplePoint - p;
    const float cosTheta = -dot(wi, normal);

    // P(wi) = r^2 / cosTheta * A
    pdf = (wi.length2() / cosTheta) * invArea;
    return (cosTheta > 0.f) ? L(p) : 0.f;
}

const rgbColor areaLight::L(const point3& p) const{
	return lightColor * power;
}

const bool areaLight::intersect(const ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(abs(D) < EPSILON){
        return false;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - A;
    const float beta = dot(dir, s1) * invD;
    if(beta < 0.f || beta > 2.f){
        return false;
    }

    const vec3 s2 = cross(B, dir);
    const float gamma = dot(r.direction, s2) * invD;
    if(gamma < 0.f || beta + gamma > 2.f){
        return false;
    }

    const float t = dot(C, s2) * invD;
	if(t < r.tMin || t >= r.tMax){
		return false;
	}

    return true;
}
