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
    area(fabs(cross(vA,vB).length())), invArea(1.f/fabs(cross(vA,vB).length()))
{}

const rgbColor areaLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const{
    point3 samplePoint;
    sampleRectangle(samplePoint, a, b, position, u0, u1);

    wi = samplePoint - p;
    const float cosTheta = fabs(dot(-wi, normal));
    if(cosTheta > 0.f){
        pd = pdf(p, wi);
        return lightColor * power;
    }else{
        pd = 0.f;
        return 0.f;
    }
}

const float areaLight::pdf(const point3& p, const vec3& wi) const {
    // P(wi) = r^2 / cosTheta * A
    const float cosTheta = fabs(dot(-wi, normal));
    const intersection isect = intersect(ray(p, normalize(wi)));

    if(cosTheta > 0.f && isect.hit){
        return (isect.t * isect.t) / (cosTheta * area);
    }else{
        return 0.f;
    }
}

const rgbColor areaLight::L(const ray& r) const{
	return lightColor * power;
}

const intersection areaLight::intersect(const ray& r) const {
    ray rorig(r);
    // Backface Culling.
    if(dot(r.direction, normal) > 0){
        return noIntersect;
    }

    intersection isect1 = triangle(A, B, C).intersect(rorig);
    if(isect1.hit){
        isect1.s = NULL;
        isect1.p = NULL;
        isect1.li = this;
        return isect1;
    }else{
        rorig = r;
        intersection isect2 = triangle(B, D, C).intersect(rorig);
        isect2.s = NULL;
        isect2.p = NULL;
        isect2.li = this;
        return isect2;
    }
}
