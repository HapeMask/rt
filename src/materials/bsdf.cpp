#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>
#include <vector>
#include <iostream>
using std::vector;
using std::cerr;
using std::endl;

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
    switch((int)b->getType()){
        case (SPECULAR | TRANSMISSION):
            if(specTra){
                delete specTra;
            }
            specTra = (specularBxdf*)b;
            break;
        case (SPECULAR | REFLECTION):
            if(specRef){
                delete specRef;
            }
            specRef = (specularBxdf*)b;
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
        default:
            cerr << "Invalid type added: " << b->getType() << endl;
            break;
    }
}

const rgbColor bsdf::f(const vec3& wo, const vec3& wi, bxdfType type) const{
    rgbColor f(0.f);

    // Ignore BTDFs if the vectors are on the same side of the surface. 
    if(wo.y * wi.y > 0){
        type = bxdfType(type & ~TRANSMISSION);
    }else{
        type = bxdfType(type & ~REFLECTION);
    }

    if(isSubtype(REFLECTION, type)){
        if(isSubtype(DIFFUSE, type) && diffRef) f += diffRef->f(wo, wi);
        if(isSubtype(SPECULAR, type) && specRef) f += specRef->f(wo, wi);
        if(isSubtype(GLOSSY, type) && glossRef) f += glossRef->f(wo, wi);
    }

    if(isSubtype(TRANSMISSION, type)){
        if(isSubtype(DIFFUSE, type) && diffTra) f += diffTra->f(wo, wi);
        if(isSubtype(SPECULAR, type) && specTra) f += specTra->f(wo, wi);
        if(isSubtype(GLOSSY, type) && glossTra) f += glossTra->f(wo, wi);
    }

    return f;
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

const rgbColor bsdf::sampleF(const float& u0, const float& u1, const float& u2,
        const vec3& wo, vec3& wi,
        bxdfType type, bxdfType& sampledType, float& p) const{
    p = 0.f;
    rgbColor f(0.f);
    vector<bxdf*> matches;

    // Find all matching bxdfs.
    if(isSubtype(REFLECTION, type)){
        if(isSubtype(DIFFUSE, type) && diffRef){
            matches.push_back(diffRef);
        }
        if(isSubtype(SPECULAR, type) && specRef){
            matches.push_back(specRef);
        }
        if(isSubtype(GLOSSY, type) && glossRef){
            matches.push_back(glossRef);
        }
    }

    if(isSubtype(TRANSMISSION, type)){
        if(isSubtype(DIFFUSE, type) && diffTra) {
            matches.push_back(diffTra);
        }
        if(isSubtype(SPECULAR, type) && specTra) {
            matches.push_back(specTra);
        }
        if(isSubtype(GLOSSY, type) && glossTra) {
            matches.push_back(glossTra);
        }
    }

    if(matches.size() == 0){
        p = 0.f;
        return rgbColor(0.f);
    }

    /*
     * Specular importance sampling I think.
     */
	///*
    if(specRef && specTra && matches.size() > 1){
        const rgbColor F = specRef->evalFresnel(fabsf(bsdf::cosTheta(wo)));
        const float Fr = (F.red() + F.blue() + F.green())/3.f;
        const float Ft = 1.f - Fr;

        if(u0 < Ft){
            f = specTra->sampleF(u1, u2, wo, wi, p);
            sampledType = specTra->getType();
			p = Ft;
        }else{
            f = specRef->sampleF(u1, u2, wo, wi, p);
            sampledType = specRef->getType();
			p = Fr;
        }
        return f;
    }
	//*/

    // Select and sample a random bxdf component to find wi.
    const unsigned int index = sampleRange(u0, 0, matches.size()-1);
    f = matches[index]->sampleF(u1, u2, wo, wi, p);
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
				f += matches[i]->f(wo, wi);
			}
        }
    }

    return f;
}

testBsdf::testBsdf() {
    glossRef = new microfacetBrdf(1.8f, 1.f, new blinn(rgbColor(0.9f,0.9f,0.9f), 50));
    //glossRef = new lambertianBrdf(rgbColor(0,0,0));
    glossTra = new microfacetBtdf(1.8f, 1.f, new blinn(rgbColor(64, 128, 255), 50));
    //glossTra = new lambertianBrdf(rgbColor(0,0,0));
}
