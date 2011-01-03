#include "arealight.hpp"
#include "samplers/samplers.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"

areaLight::areaLight(const point3& p, const float& pow, const rgbColor& c,
        const vec3& vA, const vec3& vB) :
    light(p, pow, c), a(vA), b(vB), normal(normalize(cross(vA,vB))),
	tri1(point3(p - 0.5f*vA - 0.5f*vB), point3(p + 0.5f*vA - 0.5f*vB), point3(p + 0.5f*vB - 0.5f*vA)),
	tri2(point3(p + 0.5f*vA - 0.5f*vB), point3(p + 0.5f*vB + 0.5f*vA), point3(p + 0.5f*vB - 0.5f*vA)),
    area(fabsf(norm(cross(vA,vB)))), invArea(1.f/fabsf(norm(cross(vA,vB))))
{}

const rgbColor areaLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const{
    point3 samplePoint;
    sampleRectangle(samplePoint, a, b, position, u0, u1);

    wi = samplePoint - p;
    const float cosTheta = dot(-wi, normal);
    if(cosTheta > 0){
        pd = norm2(wi) / (cosTheta * area);
        return lightColor * power;
    }else{
        pd = 0.f;
        return rgbColor(0.f);
    }
}

const float areaLight::pdf(const point3& p, const vec3& wi) const {
    const intersection isect = intersect(ray(p, normalize(wi)));

    if(isect.hit){
        // P(wi) = r^2 / cosTheta * A
        const float cosTheta = fabsf(dot(-wi, normal));
        return (isect.t * isect.t) / (cosTheta * area);
    }else{
        return 0.f;
    }
}

const intersection areaLight::intersect(const ray& r) const {
    ray rorig(r);

    // Backface Culling.
    if(dot(-r.direction, normal) < 0){
        return noIntersect;
    }

    intersection isect1 = tri1.intersect(rorig);
    if(isect1.hit){
        isect1.li = this;
        return isect1;
    }else{
        rorig = r;
        intersection isect2 = tri2.intersect(rorig);
        isect2.li = this;
        return isect2;
    }
}

const bool areaLight::intersectB(const ray& r) const{
	return (tri1.intersectB(r) || tri2.intersectB(r));
}
