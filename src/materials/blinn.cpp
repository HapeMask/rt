#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor blinnMicrofacet::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float phiH = u1 * TWOPI;
    const float cosThetaH = powf(u0, 1.f / (exp+1.f));
    const float sinThetaH = sqrtf(max(0.f, 1.f - cosThetaH*cosThetaH));

    // Convert from spherical coords to BSDF xyz coords.
    const vec3 wh = vec3(sinThetaH*cosThetaH, cosf(phiH), sinThetaH*sinThetaH);
    wi = -wo + 2.f * dot(wo, wh) * wh;

    pd = ((exp + 2.f) * powf(cosThetaH, exp)) / (4.f * TWOPI * dot(wo, wh));
    return f(wo, wi);
}

const float blinnMicrofacet::pdf(const vec3& wo, const vec3& wi) const {
    const vec3 wh = halfVector(wo, wi);
    return ((exp + 2.f) * powf(bsdf::cosTheta(wh), exp)) / (4.f * TWOPI * dot(wo, wh));
}
