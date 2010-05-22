#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>
#include <vector>
using std::vector;

const rgbColor bxdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const {
    //uniformSampleHemisphere(wi);
    cosineSampleHemisphere(wi, u0, u1);
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

const rgbColor bsdf::f(const vec3& wo, const vec3& wi, bxdfType type) const{
    rgbColor f(0.f);

    // Ignore BTDFs if the vectors are on the same side of the surface. 
    if(wo.y() * wi.y() > 0){
        type = bxdfType(type & ~TRANSMISSION);
    }else{
        type = bxdfType(type & ~REFLECTION);
    }

    if(isSupertype(REFLECTION, type)){
        if(isSupertype(DIFFUSE, type) && diffRef) f += diffRef->f(wo, wi);
        if(isSupertype(SPECULAR, type) && specRef) f += specRef->f(wo, wi);
        if(isSupertype(GLOSSY, type) && glossRef) f += glossRef->f(wo, wi);
    }

    if(isSupertype(TRANSMISSION, type)){
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
        const vec3& wo, vec3& wi, bxdfType type, bxdfType& matchedType) const{
    float p;
    const rgbColor f = sampleF(u0, u1, u2, wo, wi, type, matchedType, p);
    return (p > 0.f) ? f / p : f;
}

const rgbColor bsdf::sampleF(const float& u0, const float& u1, const float& u2,
        const vec3& wo, vec3& wi, bxdfType type, bxdfType& matchedType, float& p) const{
    p = 0.f;
    rgbColor f(0.f);
    vector<bxdf*> matches;

    // Find all matching bxdfs.
    if(isSupertype(REFLECTION, type)){
        if(isSupertype(DIFFUSE, type) && diffRef){
            matches.push_back(diffRef);
        }
        if(isSupertype(SPECULAR, type) && specRef){
            matches.push_back(specRef);
        }
        if(isSupertype(GLOSSY, type) && glossRef){
            matches.push_back(glossRef);
        }
    }
    if(isSupertype(TRANSMISSION, type)){
        if(isSupertype(DIFFUSE, type) && diffTra) {
            matches.push_back(diffTra);
        }
        if(isSupertype(SPECULAR, type) && specTra) {
            matches.push_back(specTra);
        }
        if(isSupertype(GLOSSY, type) && glossTra) {
            matches.push_back(glossTra);
        }
    }

    if(matches.size() == 0){
        return 0.f;
    }

    // Select and sample a random bxdf component to find wi.
    const unsigned int index = sampleRange(0, matches.size()-1);
    f = matches[index]->sampleF(u1, u2, wo, wi, p);
    matchedType = matches[index]->getType();

    // If it was a specular bxdf, then we just take the value from f
    // and ignore the others as well as the pdfs, as the specular components
    // have delta distributions for the pdfs.
    if(!isSupertype(SPECULAR, matchedType) && matches.size() > 1){
        // p currently contains the pdf for the sampled bxdf,
        // we still need to add the other contributions.
        //
        // Also evaluate and add the bsdf component values.

        f = 0.f;
        for(size_t i=0; i<matches.size(); ++i){
            if(i != index){
                p += matches[i]->pdf(wo, wi);
            }

            f += matches[i]->f(wo, wi);
        }

        p /= (float)matches.size();
    }

    if(matches.size() > 1){
        p /= (float)matches.size();
    }

    return f;
}

const rgbColor lambertianBrdf::f(const vec3& wo, const vec3& wi) const {
	return rOverPi;
}

const rgbColor specularBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    pd = 1.f;
    wi = vec3(-wo.x(), wo.y(), -wo.z());

    const float Fr = evalFresnel(fabs(bsdf::cosTheta(wo)));
    return Fr * kR / fabs(bsdf::cosTheta(wi));
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
    const float eta = entering ? (1.f / ior) : ior;
    const float eta2 = eta*eta;

    const float sin2ThetaT = eta2 * bsdf::sin2Theta(wo);
    // Total Internal Reflection
    if(sin2ThetaT > 1.f){
        return 0.f;
    }

    // Flip the normal if we're entering the surface.
    const float cosThetaT = entering ?
        -sqrtf(max(0.f, 1.f - sin2ThetaT)) :
        sqrtf(max(0.f, 1.f - sin2ThetaT));

    wi.x() = eta * -wo.x();
    wi.y() = cosThetaT;
    wi.z() = eta * -wo.z();

    // abs(cosTheta) "flips the normal" for us.
    const float Fr = evalFresnel(fabs(bsdf::cosTheta(wo)));
    return (1.f/eta2) * (1.f - Fr) * kT / fabs(bsdf::cosTheta(wi));
}

const rgbColor phongBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float sinAlpha = sqrtf(1.f - powf(u0, 2.f / (float)(n+1)));
    const float phi = TWOPI * u1;

    wi.x() = sinAlpha * cosf(phi);
    wi.y() = powf(u1, (1.f / (float)(n+1)));
    wi.z() = sinAlpha * sinf(phi);

    // Treat N<1000 as glossy, N>=1000 as specular.
    if(n < 1000){
        pd = pdf(wo, wi);
        return f(wo, wi);
    }else{
        pd = 1.f;
        return 1.f;
    }
}

const rgbColor phongBrdf::f(const vec3& wo, const vec3& wi) const{
    // -wo.z = cos(perfect specular reflection dir)
    return ks * (float)(n+2) * INVTWOPI *
        powf(fabs(dot(wo, vec3(-wi.x(), wi.y(), -wi.z()))), n);
}

const float phongBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return ((float)(n+1)/TWOPI) * powf(fabs(dot(wo, vec3(-wi.x(), wi.y(), -wi.z()))), n);
}
