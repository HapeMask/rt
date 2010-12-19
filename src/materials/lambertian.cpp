#include "bsdf.hpp"
#include "samplers/samplers.hpp"

lambertianBrdf::lambertianBrdf(const rgbColor& r, texture2DPtr diffuseTex) :
    bxdf(bxdfType(DIFFUSE | REFLECTION)), rOverPi(r * INVPI)
{
    setTexture(DIFFUSE_COLOR, diffuseTex);
}

const rgbColor lambertianBrdf::sampleF(
        const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const {
    cosineSampleHemisphere(wi, u0, u1);
    pd = pdf(wo, wi);
    return f(wo, wi);
}

inline const rgbColor lambertianBrdf::f(const vec3& wo, const vec3& wi) const {
#ifdef RT_MULTITHREADED
    if(hasTexture) {
        return texture2D::lookupCache[omp_get_thread_num()][DIFFUSE_COLOR];
    }else {
        return rOverPi;
    }
#else
    return rOverPi;
#endif
}

inline const float lambertianBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return bsdf::cosTheta(wi) * INVPI;
}

inline void lambertianBrdf::updateFromUVTexture(const vec2& uv) {
    if(hasTexture && uv.s != -1) {
        const rgbColor c = textureLookup(DIFFUSE_COLOR, uv);
#ifdef RT_MULTITHREADED
        texture2D::lookupCache[omp_get_thread_num()][DIFFUSE_COLOR] = c;
#else
        rOverPi = c;
#endif
    }
}