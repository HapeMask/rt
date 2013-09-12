#include "bsdf.hpp"
#include "samplers/samplers.hpp"

lambertianBrdf::lambertianBrdf(texture2DPtr diffuseTex) :
    bxdf(bxdfType(DIFFUSE | REFLECTION)), rOverPi(0)
{
    setTexture(DIFFUSE_COLOR, diffuseTex);
}

rgbColor lambertianBrdf::sampleF(
        const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const {
    cosineSampleHemisphere(wi, u0, u1);
    pd = pdf(wo, wi);
    return f(wo, wi);
}

rgbColor lambertianBrdf::f(const vec3& wo, const vec3& wi) const {
#ifdef RT_USE_OPENMP
    if(hasTexture) {
        return texture2D::lookupCache[omp_get_thread_num()][DIFFUSE_COLOR];
    }else {
        return rOverPi;
    }
#else
    return rOverPi;
#endif
}

float lambertianBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return abs(bsdf::cosTheta(wi) * INVPI);
}

void lambertianBrdf::updateFromUVTexture(const vec2& uv) {
    if(hasTexture && uv.x != -1) {
        const rgbColor c = textureLookup(DIFFUSE_COLOR, uv);
#ifdef RT_USE_OPENMP
        texture2D::lookupCache[omp_get_thread_num()][DIFFUSE_COLOR] = c;
#else
        rOverPi = c;
#endif
    }
}
