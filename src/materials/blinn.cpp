#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

float blinn::D(const vec3& wh) const {
    return (exp+2.f) * INVTWOPI * pow(bsdf::cosTheta(wh), exp);
}

void blinn::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float phiH = u1 * TWOPI;
    const float cosThetaH = pow(u0, 1.f / (exp+1.f));
    const float sinThetaH = sqrt(std::max(0.f, 1.f - cosThetaH*cosThetaH));

    vec3 wh;
    sphericalToDirection(wh, sinThetaH, cosThetaH, phiH);
    wh = normalize(wh);

    if(wh.y > 0){
        wi = 2.f * abs(dot(wo, wh)) * wh - wo;
        pd = ((exp + 2.f) * pow(cosThetaH, exp)) / (4.f * TWOPI * abs(dot(wo, wh)));
    }else{
        pd = 0.f;
    }
}

float blinn::pdf(const vec3& wo, const vec3& wi) const {
    const vec3 wh = halfVector(wo, wi);
    if(wh.y > 0){
        return ((exp + 2.f) * pow(bsdf::cosTheta(wh), exp)) / (4.f * TWOPI * dot(wo, wh));
    }else{
        return 0.f;
    }
}
