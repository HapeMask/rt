#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

inline const float beckmann::G(const vec3& wo, const vec3& wi, const vec3& wh) const {
    return G1(wo, wh) * G1(wi, wh);
}

inline const float beckmann::G1(const vec3& v, const vec3& wh) const {
    if(dot(v,wh) / bsdf::cosTheta(v) > 0.f){
        const float cosThetaH = bsdf::cosTheta(wh);
        const float a = 1.f / (alpha * tanf(acosf(cosThetaH)));
        if(a < 1.6f){
            return (3.535f * a + 2.181* a*a) / (1.f + 2.276*a + 2.577 * a*a);
        }else{
            return 1.f;
        }
    }else{
        return 0.f;
    }
}

const float beckmann::D(const vec3& wh) const {
    const float cosThetaH = bsdf::cosTheta(wh);
    if(cosThetaH > 0.f){
        const float thetaH = acosf(cosThetaH);
        const float tanThetaH = tanf(thetaH);
        const float e = -(tanThetaH * tanThetaH) / (alpha * alpha);
        return (exp(e) * INVPI) / (alpha * alpha * pow(cosThetaH, 4));
    }else{
        return 0.f;
    }
}

void beckmann::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float thetaH = atanf(sqrt(-(alpha*alpha) * logf(1.f - u0)));
    const float phiH = TWOPI * u1;
    const float sinThetaH = sinf(thetaH);
    const float cosThetaH = cosf(thetaH);

    vec3 wh;
    sphericalToDirection(wh, sinThetaH, cosThetaH, phiH);
    wh.normalize();

    if(wh.y > 0){
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
