#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

float blinn::D(const vec3& wh) const {
    return (exp+2.f) * INVTWOPI * powf(bsdf::cosTheta(wh), exp);
}

void blinn::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float phiH = u1 * TWOPI;
    const float cosThetaH = powf(u0, 1.f / (exp+1.f));
    const float sinThetaH = sqrtf(std::max(0.f, 1.f - cosThetaH*cosThetaH));

    vec3 wh;
    sphericalToDirection(wh, sinThetaH, cosThetaH, phiH);
    wh = normalize(wh);

    if(wh.y() > 0){
        wi = 2.f * fabsf(dot(wo, wh)) * wh - wo;

        pd = ((exp + 2.f) * powf(cosThetaH, exp)) / (4.f * TWOPI * fabsf(dot(wo, wh)));
    }else{
        pd = 0.f;
    }
}

float blinn::pdf(const vec3& wo, const vec3& wi) const {
    const vec3 wh = halfVector(wo, wi);
    if(wh.y() > 0){
        return ((exp + 2.f) * powf(bsdf::cosTheta(wh), exp)) / (4.f * TWOPI * dot(wo, wh));
    }else{
        return 0.f;
    }
}
