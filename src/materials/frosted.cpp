#include "bsdf.hpp"

frostedGlassBsdf::frostedGlassBsdf(microfacetBrdf* reflDistrib,
        microfacetBtdf* transDistrib)
{
    glossRef = reflDistrib;
    glossTra = transDistrib;
}

rgbColor frostedGlassBsdf::sampleF(const float& u0, const float& u1, const float&
        u2, const vec3& wo, vec3& wi, bxdfType type, bxdfType& sampledType,
        float& pd) const {

    const rgbColor F = glossTra->evalFresnel(abs(bsdf::cosTheta(wo)));
    const float Fr = (F.red() + F.blue() + F.green())/3.f;
    const float Ft = 1.f - Fr;
    rgbColor ret(0.f);
    pd = 0.f;

    if(isSubtype(glossTra->getType(), type) && u0 < Ft){
        ret = glossTra->sampleF(u1, u2, wo, wi, pd);
        sampledType = glossTra->getType();
        pd *= Ft;
    }else if(isSubtype(glossRef->getType(), type)){
        ret = glossRef->sampleF(u1, u2, wo, wi, pd);
        sampledType = glossRef->getType();
        pd *= Fr;
    }

    return ret;
}

rgbColor frostedGlassBsdf::f(const vec3& wo, const vec3& wi, bxdfType type) const {
    // Ignore BTDFs if the vectors are on the same side of the surface. 
    if(wo.y * wi.y > 0 && bsdf::isSubtype(glossRef->getType(), type)){
        return glossRef->f(wo, wi);
    }else if(bsdf::isSubtype(glossTra->getType(), type)){
        return glossTra->f(wo, wi);
    }else{
        return rgbColor(0.f);
    }
}

float frostedGlassBsdf::pdf(const vec3& wo, const vec3& wi, bxdfType type) const {
    float p = 0.f;
    float count = 0;

    if(bsdf::isSubtype(glossRef->getType(), type)){
        p += glossRef->pdf(wo, wi);
        ++count;
    }

    if(bsdf::isSubtype(glossTra->getType(), type)){
        p += glossTra->pdf(wo, wi);
        ++count;
    }

    return (count > 0.f) ? p / count : 0.f;
}
