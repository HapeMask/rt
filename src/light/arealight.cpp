#include "light.hpp"
#include "samplers/samplers.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"

#include "acceleration/intersection.hpp"
#include "geometry/triangle.hpp"

areaLight::areaLight(const point3& p, const float& pow, const rgbColor& c,
        const vec3& vA, const vec3& vB) :
    light(p, pow, c), a(vA), b(vB), normal(normalize(cross(vA,vB))),
    A(p - 0.5*vA - 0.5*vB), B(p + 0.5*vA - 0.5*vB), C(p + 0.5*vB - 0.5*vA), D(A + (B-A) + (C-A)),
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

const bool areaLight::intersect(ray& r) const {
    const ray rorig(r);
    // Backface Culling.
    if(dot(r.direction, normal) > 0){
        return false;
    }

    const intersection isect1 = triangle(A, B, C).intersect(r);
    if(isect1.hit){
        r.tMax = isect1.t;
        return true;
    }else{
        r = rorig;
        const intersection isect2 = triangle(B, D, C).intersect(r);
        r.tMax = isect2.t;
        return isect2.hit;
    }

    return false;
}
