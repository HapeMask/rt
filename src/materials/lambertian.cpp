#include "bsdf.hpp"
#include "samplers/samplers.hpp"

const rgbColor lambertianBrdf::sampleF(
        const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const {
    cosineSampleHemisphere(wi, u0, u1);
    pd = pdf(wo, wi);
    return f(wo, wi);
}

inline const rgbColor lambertianBrdf::f(const vec3& wo, const vec3& wi) const {
    return rOverPi;
}

inline const float lambertianBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return bsdf::cosTheta(wi) * INVPI;
}
