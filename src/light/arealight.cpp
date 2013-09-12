#include "arealight.hpp"
#include "samplers/samplers.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"

areaLight::areaLight(const point3& p, const float& pow, const rgbColor& c,
        const vec3& vA, const vec3& vB) :
    light(p, pow, c), a(vA), b(vB), normal(normalize(cross(vA,vB))),
    area(abs(norm(cross(vA,vB)))), invArea(1.f/abs(norm(cross(vA,vB))))
{
    const point3 A = p - 0.5f*vA - 0.5f*vB;
    const point3 B = p + 0.5f*vA - 0.5f*vB;
    const point3 C = p - 0.5f*vA + 0.5f*vB;
    const point3 D = p + 0.5f*vA + 0.5f*vB;

    tri1 = triangle(A, B, C);
    tri2 = triangle(B, D, C);
}

rgbColor areaLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const{
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

float areaLight::pdf(const point3& p, const vec3& wi) const {
    const intersection isect = intersect(ray(p, normalize(wi)));

    if(isect.hit){
        // P(wi) = r^2 / cosTheta * A
        const float cosTheta = abs(dot(-wi, normal));
        return (isect.t * isect.t) / (cosTheta * area);
    }else{
        return 0.f;
    }
}

intersection areaLight::intersect(const ray& r) const {
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

bool areaLight::intersectB(const ray& r) const{
	return (tri1.intersectB(r) || tri2.intersectB(r));
}
