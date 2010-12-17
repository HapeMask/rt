#include "bsdf.hpp"

inline const float microfacetDistribution::G(const vec3& wo, const vec3& wi, const vec3& wh) const {
    const float ndotwo = fabs(bsdf::cosTheta(wo));
    const float ndotwi = fabs(bsdf::cosTheta(wi));
    const float ndotwh = fabs(bsdf::cosTheta(wh));
    const float wodotwh = fabs(dot(wo, wh));
    return min(1.f, min(2.f * ndotwh * ndotwo / wodotwh,
                2.f * ndotwh * ndotwi / wodotwh));
}

const rgbColor microfacetBrdf::f(const vec3& wo, const vec3& wi) const {
    const vec3 wh = halfVector(wo, wi);
    const float cosThetaO = bsdf::cosTheta(wo);
    const float cosThetaI = bsdf::cosTheta(wi);
    const float cosThetaH = dot(wi, wh);
    const rgbColor F = rescaledApproxFresnel(eta, k, cosThetaH);

    return 
        distrib->rho * distrib->D(wh) * distrib->G(wo, wi, wh) * F /
        (4.f * cosThetaO * cosThetaI);
}

inline const rgbColor microfacetBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const {
    distrib->sampleF(u0, u1, wo, wi, pd);
    return f(wo, wi);
}

inline const float microfacetBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return distrib->pdf(wo, wi);
}

microfacetBrdf::~microfacetBrdf(){
    delete distrib;
}
