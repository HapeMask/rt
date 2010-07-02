#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

void beckmann::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float thetaH = atanf(sqrt(-(alpha*alpha) * logf(1.f - u0)));
    const float phiH = TWOPI * u1;
    const float sinThetaH = sinf(thetaH);
    const float cosThetaH = cosf(thetaH);

    vec3 wh;
    sphericalToDirection(wh, sinThetaH, cosThetaH, phiH);
    wh.normalize();

    if(wh.y() > 0){
        wi = 2.f * fabs(dot(wo, wh)) * wh - wo;
        pd = pdf(wo, wi);
    }else{
        pd = 0.f;
    }
}

const float beckmann::pdf(const vec3& wo, const vec3& wi) const{
    const vec3 wh = halfVector(wo, wi);
    return D(wh) / (4.f * fabs(dot(wo, wh)));
}
