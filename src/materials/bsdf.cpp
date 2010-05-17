#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor bxdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const {
    cosineSampleHemisphere(wi, sampleUniform(), sampleUniform());
    //cosineSampleHemisphere(wi, u0, u1);
    pd = pdf(wo, wi);
    return f(wo, wi);
}


bsdf::~bsdf(){
    if(specTra){
        delete specTra;
    }
    if(specRef){
        delete specRef;
    }
    if(diffTra){
        delete diffTra;
    }
    if(diffRef){
        delete diffRef;
    }
    if(glossTra){
        delete glossTra;
    }
    if(glossRef){
        delete glossRef;
    }
}
void bsdf::addBxdf(bxdf* b){
    switch(b->getType()){
        case (SPECULAR | TRANSMISSION):
            if(specTra){
                delete specTra;
            }
            specTra = b;
            break;
        case (SPECULAR | REFLECTION):
            if(specRef){
                delete specRef;
            }
            specRef = b;
            break;
        case (DIFFUSE | TRANSMISSION):
            if(diffTra){
                delete diffTra;
            }
            diffTra = b;
            break;
        case (DIFFUSE | REFLECTION):
            if(diffRef){
                delete diffRef;
            }
            diffRef = b;
            break;
        case (GLOSSY | TRANSMISSION):
            if(glossTra){
                delete glossTra;
            }
            glossTra = b;
            break;
        case (GLOSSY | REFLECTION):
            if(glossRef){
                delete glossRef;
            }
            glossRef = b;
            break;
    }
}

const rgbColor bsdf::f(const vec3& wi, const vec3& wo, bxdfType type) const{
    /*
    switch(type){
        case (SPECULAR | TRANSMISSION):
            if(specTra != NULL){
                return specTra->f(wo, wi);
            }
            break;
        case (SPECULAR | REFLECTION):
            if(specRef != NULL){
                return specRef->f(wo, wi);
            }
            break;
        case (DIFFUSE | TRANSMISSION):
            if(diffTra != NULL){
                return diffTra->f(wo, wi);
            }
            break;
        case (DIFFUSE | REFLECTION):
            if(diffRef != NULL){
                return diffRef->f(wo, wi);
            }
            break;
        case (GLOSSY | TRANSMISSION):
            if(glossTra != NULL){
                return glossTra->f(wo, wi);
            }
            break;
        case (GLOSSY | REFLECTION):
            if(glossRef != NULL){
                return glossRef->f(wo, wi);
            }
            break;
    }
    */

    rgbColor f(0.f);
    if(isSupertype(REFLECTION, type)){
        if(isSupertype(DIFFUSE, type) && diffRef) f += diffRef->f(wo, wi);
        if(isSupertype(SPECULAR, type) && specRef) f += specRef->f(wo, wi);
        if(isSupertype(GLOSSY, type) && glossRef) f += glossRef->f(wo, wi);
    }else{
        if(isSupertype(DIFFUSE, type) && diffTra) f += diffTra->f(wo, wi);
        if(isSupertype(SPECULAR, type) && specTra) f += specTra->f(wo, wi);
        if(isSupertype(GLOSSY, type) && glossTra) f += glossTra->f(wo, wi);
    }

    return f;
}

const float bsdf::pdf(const vec3& wo, const vec3& wi, bxdfType type) const{
    float p = 0;
    if(isSupertype(REFLECTION, type)){
        if(isSupertype(DIFFUSE, type) && diffRef){
            p += diffRef->pdf(wo, wi);
        }
        if(isSupertype(SPECULAR, type) && specRef){
            p += specRef->pdf(wo, wi);
        }
        if(isSupertype(GLOSSY, type) && glossRef){
            p += glossRef->pdf(wo, wi);
        }
    }else{
        if(isSupertype(DIFFUSE, type) && diffTra) {
            p += diffTra->pdf(wo, wi);
        }
        if(isSupertype(SPECULAR, type) && specTra) {
            p += specTra->pdf(wo, wi);
        }
        if(isSupertype(GLOSSY, type) && glossTra) {
            p += glossTra->pdf(wo, wi);
        }
    }
    return p;
}

const rgbColor bsdf::sampleF(const float& u0, const float& u1, const float& u2,
        const vec3& wo, vec3& wi, bxdfType type) const{
    float p;
    const rgbColor f = sampleF(u0, u1, u2, wo, wi, type, p);
    return (p > 0.f) ? f / p : f;
}

const rgbColor bsdf::sampleF(const float& u0, const float& u1, const float& u2,
        const vec3& wo, vec3& wi, bxdfType type, float& p) const{
    p = 0.f;
    rgbColor f(0.f);

    if(isSupertype(REFLECTION, type)){
        if(isSupertype(DIFFUSE, type) && diffRef){
            f += diffRef->sampleF(u1, u2, wo, wi, p);
            //p += diffRef->pdf(wo, wi);
        }
        if(isSupertype(SPECULAR, type) && specRef){
            f = specRef->sampleF(u1, u2, wo, wi, p);
            //p += specRef->pdf(wo, wi);
        }
        if(isSupertype(GLOSSY, type) && glossRef){
            f += glossRef->sampleF(u1, u2, wo, wi, p);
            //p += glossRef->pdf(wo, wi);
        }
    }else{
        if(isSupertype(DIFFUSE, type) && diffTra) {
            f += diffTra->sampleF(u1, u2, wo, wi, p);
            //p += diffTra->pdf(wo, wi);
        }
        if(isSupertype(SPECULAR, type) && specTra) {
            f = specTra->sampleF(u1, u2, wo, wi, p);
            //p += specTra->pdf(wo, wi);
        }
        if(isSupertype(GLOSSY, type) && glossTra) {
            f += glossTra->sampleF(u1, u2, wo, wi, p);
            //p += glossTra->pdf(wo, wi);
        }
    }

    return f;
}

const rgbColor lambertianBrdf::f(const vec3& wo, const vec3& wi) const {
	return rOverPi;
}

const rgbColor specularBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    pd = 1.f;
    wi = vec3(-wo.x(), wo.y(), -wo.z());
    return 1.f;
}

const rgbColor specularBtdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{

    pd = 1.f;
    const bool entering = (bsdf::cosTheta(wo) > 0.f);

    // The normal always points out of the object (as in PBRT), thus
    // we need to check if the ray is inside the object by checking
    // the sign of the angle between the ray and the normal.
    //
    // Swaps IOR accordingly and generates a refraction normal
    // that always points in the same direction as the ray.
    const float nr = entering ? (1.f / ior) : ior;

    const float sin2ThetaT = nr * nr * bsdf::sin2Theta(wo);
    // Total Internal Reflection
    if(sin2ThetaT > 1.f){
        return 0.f;
    }

    // Flip the normal if we're entering the surface.
    const float cosThetaT = entering ?
        -sqrtf(max(0.f, 1.f - sin2ThetaT)) :
        sqrtf(max(0.f, 1.f - sin2ThetaT));

    wi.x() = nr * -wo.x();
    wi.y() = cosThetaT;
    wi.z() = nr * -wo.z();
    wi = normalize(wi);

    return 1.f;
}

const rgbColor phongBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    return rgbColor(1.f,0.f,0.f);
    //return f(wo, wi);
}

const rgbColor phongBrdf::f(const vec3& wo, const vec3& wi) const{
    // -wo.z = cos(perfect specular reflection dir)
    return ks * ((float)(n+2)/TWOPI) *
        pow(abs(dot(wo, vec3(-wi.x(), wi.y(), -wi.z()))), n);
}
