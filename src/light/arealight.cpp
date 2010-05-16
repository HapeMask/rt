#include "light.hpp"
#include "samplers/samplers.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"

areaLight::areaLight(const point3& p, const float& pow, const rgbColor& c,
        const vec3& A, const vec3& B) :
    light(p, pow, c), a(A), b(B), normal(normalize(cross(A,B))),
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
    return false;
}
