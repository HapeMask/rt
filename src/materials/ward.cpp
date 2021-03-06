#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

rgbColor newWard::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    if(wo.y < 0.f) {
        pd = 0.f;
        return rgbColor(0.f);
    }

    const float phi = isIsotropic ? 
        TWOPI * u0 :
        atan2(beta * sin(TWOPI * u0), alpha * cos(TWOPI * u0));

    const float cosPhi = cos(phi);
    const float sinPhi = sin(phi);
    const float theta = isIsotropic ?
        atan(alpha * sqrt(-logf(u1))) :
        atan(
                                    sqrt(-logf(u1) /
                ((cosPhi*cosPhi)/(alpha*alpha) + (sinPhi*sinPhi)/(beta*beta)))
            );

    vec3 wh;
    sphericalToDirection(wh, sin(theta), cos(theta), phi);
    wh = normalize(wh);

    // Find the incident direction that corresponds to this half vector and the
    // reflected direction.
    wi = 2.f * dot(wo, wh) * wh - wo;
    if(wi.y < 0.f){
        pd = 0.f;
        return rgbColor(0.f);
    }

    // For the derivation of this PDF and the fact that it isn't the one
    // computed below, see Bruce Walter's TR from Cornell: PCG-05-06).
    //
    // Lots of cancellation between BRDF and PDF -> simpler calculations.
    pd = sqrt(wi.y*wo.y) / (abs(dot(wh, wo)) * pow(wh.y, 3));
    return Rs;
}

rgbColor newWard::f(const vec3& wo, const vec3& wi) const{
    if(wo.y*wi.y < 0.f){
        return rgbColor(0.f);
    }

    const vec3 wh = halfVector(wo, wi);

    const float meanX = wh.x / alpha;
    const float meanZ = wh.z / beta;

    return Rs * exp( -(meanX*meanX + meanZ*meanZ) / (wh.y*wh.y)) /
        (4.f * PI * alpha * beta * sqrt(wo.y*wi.y));
}

float newWard::pdf(const vec3& wo, const vec3& wi) const{
    if(wo.y*wi.y < 0.f){
        return 0.f;
    }

    const vec3 wh(halfVector(wo, wi));

    const float meanX = wh.x / alpha;
    const float meanZ = wh.z / beta;

    return exp( -(meanX*meanX + meanZ*meanZ) / (wh.y*wh.y)) /
        (4.f * PI * alpha * beta * abs(dot(wh,wo)) * pow(wh.y, 3));
}
