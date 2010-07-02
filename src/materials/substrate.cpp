#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor substrate::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    if(u0 < 0.5f){
        const float u = 2.f * u0;
        cosineSampleHemisphere(wi, u, u1);
    }else{
        const float u = 2.f * (u0 - 0.5f);
        distrib->sampleF(u, u1, wo, wi, pd);

        if(wo.y() * wi.y() < 0){
            return 0.f;
        }
    }

    pd = pdf(wo, wi);
    return f(wo, wi);
}

const rgbColor substrate::f(const vec3& wo, const vec3& wi) const{
    const rgbColor diffuse = ecTerm *
        (1.f - powf(1.f - bsdf::cosTheta(wo) * 0.5f, 5)) * 
        (1.f - powf(1.f - bsdf::cosTheta(wi) * 0.5f, 5));

    const vec3 wh = halfVector(wo, wi);
    const float wiDotWh = fabs(dot(wi, wh));

    const rgbColor specular = (distrib->D(wh) * schlickFresnel(Rs, wiDotWh)) /
        (8.f * PI * wiDotWh * max(bsdf::cosTheta(wo), bsdf::cosTheta(wi)));

    return diffuse + specular;
}

const float substrate::pdf(const vec3& wo, const vec3& wi) const{
    if(wo.y() * wi.y() >= 0){
        return 0.5f * (bsdf::cosTheta(wi) * INVPI + distrib->pdf(wo, wi));
    }else{
        return 0.f;
    }
}
