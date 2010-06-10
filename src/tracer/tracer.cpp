#include "tracer.hpp"
#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"
#include "utility.hpp"

const rgbColor rayTracer::sampleOneLight(const point3& p, const vec3& wo, const intersection& isect,
        const bsdf& bsdf) const{
    if(parent->numLights() > 0){
        const unsigned int i = sampleRange(sampleUniform(), 0, parent->numLights()-1);
        return sampleDirect(p, wo, isect, bsdf, *parent->getLight(i).get()) * parent->numLights();
    }else{
        return 0.f;
    }
}

const rgbColor rayTracer::sampleAllLights(const point3& p, const vec3& wo, const intersection& isect,
        const bsdf& bsdf) const{
    if(parent->numLights() > 0){
        rgbColor L(0.f);
        for(unsigned int i=0;i<parent->numLights(); ++i){
            L += sampleDirect(p, wo, isect, bsdf, *parent->getLight(i).get()) * parent->numLights();
        }

        return L / parent->numLights();
    }else{
        return 0.f;
    }
}

const rgbColor rayTracer::sampleDirect(const point3& p, const vec3& wo,
        const intersection& isect, const bsdf& bsdf, const light& li) const {
    vec3 wi;
    float bsdfPdf, lightPdf;
    rgbColor lightSample(0.f), bsdfSample(0.f);
    float lightWeight, bsdfWeight;

    // Sample the light to find a point on the surface and the emission
    // at that point.
    const rgbColor Li = li.sampleL(p, wi, sampleUniform(), sampleUniform(), lightPdf);

    const float lightDist = wi.length();
    wi = normalize(wi);

    // Evaluate the BSDF using the direction sampled from the light.
    if(lightPdf > 0 && !Li.isBlack()){
        const vec3 bsdfSpaceLightDir = worldToBsdf(wi, isect);
        const rgbColor f = bsdf.f(wo, bsdfSpaceLightDir);

        if(!f.isBlack()){
            ray shadowRay(p, wi);
            shadowRay.tMax = lightDist;

            if(!parent->intersectB(shadowRay)){
                if(li.isPointSource()){
                    return f * Li * fabs(dot(wi, isect.shadingNormal)) / lightPdf;
                }else if(dot(-wi, li.getNormal()) < 0.f){
                    bsdfPdf = bsdf.pdf(wo, bsdfSpaceLightDir);

                    lightWeight = powerHeuristic(1, lightPdf, 1, bsdfPdf);
                    lightSample = (Li / lightPdf) * f * fabs(dot(wi, isect.shadingNormal));
                }
            }
        }
    }

    // Evaluate the BSDF using a direction sampled from the BSDF.
    if(!li.isPointSource()){
        bxdfType sampledType;
        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(), wo, wi, bxdfType(ALL & ~SPECULAR), sampledType, bsdfPdf);
        wi = normalize(bsdfToWorld(wi, isect));

        if(!f.isBlack() && bsdfPdf > 0.f){
            lightPdf = li.pdf(p, wi);
            if(lightPdf > 0.f){
                ray lightRay(p, wi);

                const intersection isectL = li.intersect(lightRay);
                lightRay.tMax = isectL.t;
                if(!parent->intersectB(lightRay)){
                    const rgbColor Li(li.L(lightRay));

                    if(!Li.isBlack()){
                        bsdfWeight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
                        bsdfSample = Li * (f / bsdfPdf) * fabs(dot(wi, isect.shadingNormal));
                    }
                }
            }
        }
    }

    // Combine the samples based on the weight from the chosen heuristic.
    return lightWeight * lightSample + bsdfWeight * bsdfSample;
}

