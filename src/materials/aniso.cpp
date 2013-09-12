#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

float aniso::D(const vec3& wh) const {
    return ecTerm * pow(abs(bsdf::cosTheta(wh)), exponent(wh));
}

float aniso::exponent(const vec3& wh) const {
    return ((nu * wh.x * wh.x) + (nv * wh.z * wh.z)) / (1.f - wh.y * wh.y);
}

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

    const float sintheta = sqrt(std::max(0.f, 1.f - costheta*costheta));
    vec3 wh;
    sphericalToDirection(wh, sintheta, costheta, phi);
    wh = normalize(wh);

    wi = 2.f * abs(dot(wo, wh)) * wh - wo;
    pd = pdf(wo, wi);

    if(wo.y * wi.y < 0.f){
        pd = 0.f;
    }
}

float aniso::pdf(const vec3& wo, const vec3& wi) const{
    const vec3 wh = halfVector(wo, wi);
    return D(wh) / (4.f * abs(dot(wo, wh)));
}
