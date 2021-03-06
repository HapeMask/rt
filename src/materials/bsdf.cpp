#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>
#include <vector>
using std::vector;

void bsdf::updateFromUVTexture(const vec2& uv) {
    if(diffRef) {
        diffRef->updateFromUVTexture(uv);
    }
    if(glossRef) {
        glossRef->updateFromUVTexture(uv);
    }
    if(specRef) {
        specRef->updateFromUVTexture(uv);
    }

    if(diffTra) {
        diffTra->updateFromUVTexture(uv);
    }
    if(glossTra) {
        glossTra->updateFromUVTexture(uv);
    }
    if(specTra) {
        specTra->updateFromUVTexture(uv);
    }
}

void bsdf::addBxdf(bxdf* b){
    switch(b->getType()){
        case (SPECULAR | TRANSMISSION):
            specTra.reset(static_cast<specularBxdf*>(b));
            break;
        case (SPECULAR | REFLECTION):
            specRef.reset(static_cast<specularBxdf*>(b));
            break;
        case (DIFFUSE | TRANSMISSION):
            diffTra.reset(b);
            break;
        case (DIFFUSE | REFLECTION):
            diffRef.reset(b);
            break;
        case (GLOSSY | TRANSMISSION):
            glossTra.reset(b);
            break;
        case (GLOSSY | REFLECTION):
            glossRef.reset(b);
            break;
        default:
            rt_throw("Invalid BSDF type added.");
            break;
    }
}

rgbColor bsdf::f(const vec3& wo, const vec3& wi, bxdfType type) const{
    rgbColor ret(0.f);

    // Ignore BTDFs if the vectors are on the same side of the surface. 
    if(wo.y * wi.y > 0){
        type = bxdfType(type & ~TRANSMISSION);
    }else{
        type = bxdfType(type & ~REFLECTION);
    }

    if(isSubtype(REFLECTION, type)){
        if(isSubtype(DIFFUSE, type) && diffRef) ret += diffRef->f(wo, wi);
        if(isSubtype(SPECULAR, type) && specRef) ret += specRef->f(wo, wi);
        if(isSubtype(GLOSSY, type) && glossRef) ret += glossRef->f(wo, wi);
    }

    if(isSubtype(TRANSMISSION, type)){
        if(isSubtype(DIFFUSE, type) && diffTra) ret += diffTra->f(wo, wi);
        if(isSubtype(SPECULAR, type) && specTra) ret += specTra->f(wo, wi);
        if(isSubtype(GLOSSY, type) && glossTra) ret += glossTra->f(wo, wi);
    }

    return ret;
}

float bsdf::pdf(const vec3& wo, const vec3& wi, bxdfType type) const{
    float p = 0;
    int matches = 0;

    if(isSubtype(REFLECTION, type)){
        if(isSubtype(DIFFUSE, type) && diffRef){
            p += diffRef->pdf(wo, wi);
            ++matches;
        }
        if(isSubtype(SPECULAR, type) && specRef){
            p += specRef->pdf(wo, wi);
            ++matches;
        }
        if(isSubtype(GLOSSY, type) && glossRef){
            p += glossRef->pdf(wo, wi);
            ++matches;
        }
    }

    if(isSubtype(TRANSMISSION, type)){
        if(isSubtype(DIFFUSE, type) && diffTra) {
            p += diffTra->pdf(wo, wi);
            ++matches;
        }
        if(isSubtype(SPECULAR, type) && specTra) {
            p += specTra->pdf(wo, wi);
            ++matches;
        }
        if(isSubtype(GLOSSY, type) && glossTra) {
            p += glossTra->pdf(wo, wi);
            ++matches;
        }
    }

    return matches > 0 ? p / (float)matches : 0.f;
}

rgbColor bsdf::sampleF(const float& u0, const float& u1, const float& u2,
        const vec3& wo, vec3& wi,
        bxdfType type, bxdfType& sampledType, float& p) const{
    p = 0.f;
    rgbColor ret(0.f);
    vector<const bxdf*> matches;

    // Find all matching bxdfs.
    if(isSubtype(REFLECTION, type)){
        if(isSubtype(DIFFUSE, type) && diffRef){
            matches.push_back(diffRef.get());
        }
        if(isSubtype(SPECULAR, type) && specRef){
            matches.push_back(specRef.get());
        }
        if(isSubtype(GLOSSY, type) && glossRef){
            matches.push_back(glossRef.get());
        }
    }

    if(isSubtype(TRANSMISSION, type)){
        if(isSubtype(DIFFUSE, type) && diffTra) {
            matches.push_back(diffTra.get());
        }
        if(isSubtype(SPECULAR, type) && specTra) {
            matches.push_back(specTra.get());
        }
        if(isSubtype(GLOSSY, type) && glossTra) {
            matches.push_back(glossTra.get());
        }
    }

    if(matches.empty()){ return ret; }

    /*
     * Specular importance sampling I think.
     */
    ///*
    if(specRef && specTra && matches.size() > 1){
        const rgbColor F = specRef->evalFresnel(abs(bsdf::cosTheta(wo)));
        const float Fr = (F.red() + F.blue() + F.green())/3.f;
        const float Ft = 1.f - Fr;

        if(u0 < Ft){
            ret = specTra->sampleF(u1, u2, wo, wi, p);
            sampledType = specTra->getType();
            p = Ft;
        }else{
            ret = specRef->sampleF(u1, u2, wo, wi, p);
            sampledType = specRef->getType();
            p = Fr;
        }
        return ret;
    }
    //*/

    // Select and sample a random bxdf component to find wi.
    const unsigned int index = sampleRange(u0, 0, matches.size()-1);
    ret = matches[index]->sampleF(u1, u2, wo, wi, p);
    sampledType = matches[index]->getType();

    // If it was a specular bxdf, then we just take the value from f
    // and ignore the others as well as the pdfs, as the specular components
    // have delta distributions for the pdfs.
    if(!(sampledType & SPECULAR) && matches.size() > 1){
        // p currently contains the pdf for the sampled bxdf,
        // we still need to add the other contributions.
        for(size_t i=0; i<matches.size(); ++i){
            if(i != index){
                p += matches[i]->pdf(wo, wi);
            }
        }
    }

    if(matches.size() > 1){
        p /= (float)matches.size();
    }

    // Evaluate and add the bsdf component values.
    if(!(sampledType & SPECULAR)){
        for(size_t i=0; i<matches.size(); ++i){
            if(i != index){
                ret += matches[i]->f(wo, wi);
            }
        }
    }

    return ret;
}

testBsdf::testBsdf() {
    glossRef.reset(new microfacetBrdf(1.8f, 1.f, new blinn(rgbColor(0.9f,0.9f,0.9f), 50)));
    glossTra.reset(new microfacetBtdf(1.8f, 1.f, new blinn(rgbColor(64, 128, 255), 50)));
}
