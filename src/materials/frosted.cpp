#include "bsdf.hpp"

frostedGlassBsdf::frostedGlassBsdf(microfacetBrdf* reflDistrib,
        microfacetBtdf* transDistrib)
{
    glossRef = reflDistrib;
    glossTra = transDistrib;
}

const rgbColor frostedGlassBsdf::sampleF(const float& u0, const float& u1, const float&
        u2, const vec3& wo, vec3& wi, bxdfType type, bxdfType& sampledType,
        float& pd) const {

    const rgbColor F = rescaledApproxFresnel(glossTra->eta, glossTra->k, fabsf(bsdf::cosTheta(wo)));
    const float Fr = (F.red() + F.blue() + F.green())/3.f;
    const float Ft = 1.f - Fr;
    rgbColor f(0.f);
    pd = 0.f;

    if((glossTra->getType() & type) && u0 < Ft){
        f = glossTra->sampleF(u1, u2, wo, wi, pd);
        sampledType = glossTra->getType();
        pd *= Ft;
    }else if(glossRef->getType() & type){
        f = glossRef->sampleF(u1, u2, wo, wi, pd);
        sampledType = glossRef->getType();
        pd *= Fr;
    }

    return f;
}

const rgbColor frostedGlassBsdf::f(const vec3& wo, const vec3& wi, bxdfType type) const {
    rgbColor f(0.f);

    // Ignore BTDFs if the vectors are on the same side of the surface. 
    if(wo.y * wi.y > 0){
        type = bxdfType(type & ~TRANSMISSION);
    }else{
        type = bxdfType(type & ~REFLECTION);
    }

    if(bsdf::isSupertype(REFLECTION, type)){
        if(bsdf::isSupertype(GLOSSY, type) && glossRef) f += glossRef->f(wo, wi);
    }

    if(bsdf::isSupertype(TRANSMISSION, type)){
        if(bsdf::isSupertype(GLOSSY, type) && glossTra) f += glossTra->f(wo, wi);
    }

    return f;
}

const float frostedGlassBsdf::pdf(const vec3& wo, const vec3& wi, bxdfType type) const {
    float p = 0.f;
    float count = 0;

    if(bsdf::isSupertype(glossRef->getType(), type)){
        p += glossRef->pdf(wo, wi);
        ++count;
    }

    if(bsdf::isSupertype(glossTra->getType(), type)){
        p += glossTra->pdf(wo, wi);
        ++count;
    }

    return (count > 0.f) ? p / count : 0.f;
}
