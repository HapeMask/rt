#include "tracer.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"

#include "acceleration/intersection.hpp"
#include "scene/scene.hpp"

#include "mathlib/vector.hpp"

rgbColor rayTracer::sampleOneLight(const point3& p, const vec3& wo, const intersection& isect,
        const bsdf& bsdf) const{
    if(parent.numLights() > 0){
        const int i = sampleRange(sampleUniform(), 0, parent.numLights()-1);
        return sampleDirect(p, wo, isect, bsdf, parent.getLight(i)) * parent.numLights();
    }else{
        return rgbColor(0.f);
    }
}

rgbColor rayTracer::sampleAllLights(const point3& p, const vec3& wo, const intersection& isect,
        const bsdf& bsdf) const{
    if(parent.numLights() > 0){
        rgbColor Li(0.f);
        for(int i=0;i<parent.numLights(); ++i){
            Li += sampleDirect(p, wo, isect, bsdf, parent.getLight(i)) * parent.numLights();
        }

        return Li / parent.numLights();
    }else{
        return rgbColor(0.f);
    }
}

rgbColor rayTracer::sampleDirect(const point3& p, const vec3& wo,
        const intersection& isect, const bsdf& bsdf, const light& li) const {
    // Ignore lights.
    if(isect.li){
        return rgbColor(0.f);
    }

    vec3 wi;
    float bsdfPdf, lightPdf;
    rgbColor lightSample(0.f), bsdfSample(0.f);
    float lightWeight = 0.f, bsdfWeight = 0.f;

    // Sample the light to find a point on the surface and the emission
    // at that point.
    const rgbColor LiL = li.sampleL(p, wi, sampleUniform(), sampleUniform(), lightPdf);

    const float lightDist = norm(wi);
    wi = normalize(wi);

    // Evaluate the BSDF using the direction sampled from the light.
    if(lightPdf > 0 && !LiL.isBlack()){
        const vec3 bsdfSpaceLightDir = worldToBsdf(wi, isect);
        const rgbColor f = bsdf.f(wo, bsdfSpaceLightDir);

        if(!f.isBlack()){
            ray shadowRay(p, wi);
            shadowRay.tMax = lightDist;

            if(!parent.intersectB(shadowRay)){
                if(li.isPointSource()){
                    return f * LiL * abs(dot(wi, isect.shadingNormal)) / lightPdf;
                }else{
                    bsdfPdf = bsdf.pdf(wo, bsdfSpaceLightDir);

                    lightWeight = powerHeuristic(1, lightPdf, 1, bsdfPdf);
                    lightSample = (LiL / lightPdf) * f * abs(dot(wi, isect.shadingNormal));
                }
            }
        }
    }

    // Evaluate the BSDF using a direction sampled from the BSDF.
    if(!li.isPointSource()){
        bxdfType sampledType;
        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(),
                wo, wi, bxdfType(ALL & ~SPECULAR), sampledType, bsdfPdf);
        wi = normalize(bsdfToWorld(wi, isect));

        if(!f.isBlack() && bsdfPdf > 0.f){
            lightPdf = li.pdf(p, wi);
            if(lightPdf > 0.f){
                ray lightRay(p, wi);

                const intersection isectL = li.intersect(lightRay);
                lightRay.tMax = isectL.t;
                if(!parent.intersectB(lightRay)){
                    const rgbColor LiB = li.L(lightRay);

                    if(!LiB.isBlack()){
                        bsdfWeight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
                        bsdfSample = LiB * (f / bsdfPdf) * abs(dot(wi, isect.shadingNormal));
                    }
                }
            }
        }
    }

    // Combine the samples based on the weight from the chosen heuristic.
    return lightWeight * lightSample + bsdfWeight * bsdfSample;
}

