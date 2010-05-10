#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"

#include <cmath>

bsdf::~bsdf(){
    if(specTra != NULL){
        delete specTra;
    }
    if(specRef != NULL){
        delete specRef;
    }
    if(diffTra != NULL){
        delete diffTra;
    }
    if(diffRef != NULL){
        delete diffRef;
    }
    if(glossTra != NULL){
        delete glossTra;
    }
    if(glossRef != NULL){
        delete glossRef;
    }
}
void bsdf::addBxdf(bxdf* b){
    switch(b->getType()){
        case (SPECULAR | TRANSMISSION):
            if(specTra != NULL){
                delete specTra;
            }
            specTra = b;
            break;
        case (SPECULAR | REFLECTION):
            if(specRef != NULL){
                delete specRef;
            }
            specRef = b;
            break;
        case (DIFFUSE | TRANSMISSION):
            if(diffTra != NULL){
                delete diffTra;
            }
            diffTra = b;
            break;
        case (DIFFUSE | REFLECTION):
            if(diffRef != NULL){
                delete diffRef;
            }
            diffRef = b;
            break;
        case (GLOSSY | TRANSMISSION):
            if(glossTra != NULL){
                delete glossTra;
            }
            glossTra = b;
            break;
        case (GLOSSY | REFLECTION):
            if(glossRef != NULL){
                delete glossRef;
            }
            glossRef = b;
            break;
    }
}

const rgbColor bsdf::f(const vec3& wi, const vec3& wo, bxdfType type) const{
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

    return rgbColor(0.f);
}

const rgbColor bsdf::sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi, bxdfType type) const{
    switch(type){
        case (SPECULAR | TRANSMISSION):
            if(specTra != NULL){
                return specTra->sampleF(u1, u2, wo, wi);
            }
            break;
        case (SPECULAR | REFLECTION):
            if(specRef != NULL){
                return specRef->sampleF(u1, u2, wo, wi);
            }
            break;
        case (DIFFUSE | TRANSMISSION):
            if(diffTra != NULL){
                return diffTra->sampleF(u1, u2, wo, wi);
            }
            break;
        case (DIFFUSE | REFLECTION):
            if(diffRef != NULL){
                return diffRef->sampleF(u1, u2, wo, wi);
            }
            break;
        case (GLOSSY | TRANSMISSION):
            if(glossTra != NULL){
                return glossTra->sampleF(u1, u2, wo, wi);
            }
            break;
        case (GLOSSY | REFLECTION):
            if(glossRef != NULL){
                return glossRef->sampleF(u1, u2, wo, wi);
            }
            break;
    }

    return rgbColor(0.f);
}

const rgbColor lambertianBrdf::f(const vec3& wo, const vec3& wi) const {
	return rOverPi;
}

const rgbColor lambertianBrdf::sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const{
    return rOverPi;
}

const rgbColor specularBrdf::sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const{
    wi = vec3(-wo.x(), wo.y(), -wo.z());
    return rgbColor(1,1,1);
}

const rgbColor specularBtdf::sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const{
    const bool entering = (bsdfCosTheta(wo) > 0.f);

    // The normal always points out of the object (as in PBRT), thus
    // we need to check if the ray is inside the object by checking
    // the sign of the angle between the ray and the normal.
    //
    // Swaps IOR accordingly and generates a refraction normal
    // that always points in the same direction as the ray.
    const float nr = entering ? (1.f / ior) : ior;

    const float sin2ThetaT = nr * nr * bsdfSin2Theta(wo);
    // Total Internal Reflection
    if(sin2ThetaT > 1.f){
        return rgbColor(0.f);
    }

    // Flip the normal if we're entering the surface.
    const float cosThetaT = entering ?
        -sqrtf(max(0.f, 1.f - sin2ThetaT)) :
        sqrtf(max(0.f, 1.f - sin2ThetaT));

    wi.x() = nr * -wo.x();
    wi.y() = cosThetaT;
    wi.z() = nr * -wo.z();
    wi = normalize(wi);

    return rgbColor(1.f);
}
