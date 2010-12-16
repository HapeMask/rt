#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

void aniso::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    // Generate a vector wh with distribution described by pdf().
    float phi, costheta;
    if (u0 < .25f) {
        sampleFirstQuadrant(4.f * u0, u1, nu, nv, phi, costheta);
    } else if (u0 < .5f) {
        const float u = 4.f * (.5f - u0);
        sampleFirstQuadrant(u, u1, nu, nv, phi, costheta);
        phi = PI - phi;
    } else if (u0 < .75f) {
        const float u = 4.f * (u0 - .5f);
        sampleFirstQuadrant(u, u1, nu, nv, phi, costheta);
        phi += PI;
    } else {
        const float u = 4.f * (1.f - u0);
        sampleFirstQuadrant(u, u1, nu, nv, phi, costheta);
        phi = TWOPI - phi;
    }

    const float sintheta = sqrtf(max(0.f, 1.f - costheta*costheta));
    vec3 wh;
    sphericalToDirection(wh, sintheta, costheta, phi);
    wh.normalize();

    wi = -wo + 2.f * fabs(dot(wo, wh)) * wh;
    pd = pdf(wo, wi);

    if(wo.y * wi.y < 0.f){
		pd = 0.f;
    }
}

const float aniso::pdf(const vec3& wo, const vec3& wi) const{
    const vec3 wh = halfVector(wo, wi);
    return D(wh) / (4.f * fabs(dot(wo, wh)));
}
