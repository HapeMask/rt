#include "path.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"

const rgbColor pathTracer::L(const ray& r) const {
    ray r2(r);
    return _L(r2);
}

const rgbColor pathTracer::_L(ray& r, const int& depth) const {
    rgbColor throughput = 1.f, L = 0.f;
    bool lastBounceWasSpecular = false;

    for(unsigned int pathLength = 0; ; ++pathLength){
        // Copy the ray since we need the original for the light test below and
        // scene::intersect() modifies it.
        const ray rCopy(r);
        const intersection isect = parent->intersect(r);
        //return rgbColor(0, isect.debugInfo / 2400.f, 0);

        if(!isect.hit || isect.p == NULL){
            if(pathLength == 0 || lastBounceWasSpecular){
                for(unsigned int i=0; i<parent->numLights(); ++i){
                    const float lightDist = (parent->getLight(i)->getPosition() - r.origin).length();
                    ray lightRay(rCopy, EPSILON, lightDist);

                    const intersection isectL = parent->getLight(i)->intersect(rCopy);
                    if(isectL.hit && !parent->intersectB(lightRay)){
                        L += throughput * isectL.li->L(rCopy);
                    }
                }
            }

            //L += throughput * rgbColor(0.5f);
            break;
        }

        const material& mat = *isect.s->getMaterial().get();
        const vec3& normal = isect.shadingNormal;
        const bsdf& bsdf = mat.getBsdf();
        const vec3 wo = worldToBsdf(-r.direction, isect);

        L += throughput * sampleOneLight(r.origin, wo, isect, bsdf);

        vec3 wi;
        float pdf = 0.f;

        bxdfType sampledType;

        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(),wo, wi, ALL, sampledType, pdf);
        if(f.isBlack() || pdf == 0.f){
            break;
        }

        wi = normalize(bsdfToWorld(wi, isect));

        throughput *= f * fabs(dot(wi, normal)) / pdf;
        lastBounceWasSpecular = (sampledType & SPECULAR) != 0;

        if(pathLength > 3){
            const float continueProbability = 0.5f;
            if(sampleUniform() > continueProbability){
                break;
            }

            throughput /= continueProbability;
        }

        if(pathLength == MAXDEPTH){
            break;
        }

        r.direction = wi;
        r.invDir = 1.f/wi;
        r.tMax = MAX_FLOAT;
        r.tMin = EPSILON;
    }

    return clamp(L);
}

const rgbColor pathTracer::sampleOneLight(const point3& p, const vec3& wo, const intersection& isect,
        const bsdf& bsdf) const{
    if(parent->numLights() > 0){
        const unsigned int i = sampleRange(sampleUniform(), 0, parent->numLights()-1);
        return sampleDirect(p, wo, isect, bsdf, *parent->getLight(i).get()) * parent->numLights();
    }else{
        return 0.f;
    }
}

const rgbColor pathTracer::sampleDirect(const point3& p, const vec3& wo,
        const intersection& isect, const bsdf& bsdf, const light& li) const {
    vec3 wi;
    float bsdfPdf, lightPdf;
    rgbColor Ld(0.f);

    //float sample[2];
    //getNextSample(sample);
    //rgbColor Li = li.sampleL(p, wi, sample[0], sample[1], lightPdf);
    rgbColor Li = li.sampleL(p, wi, sampleUniform(), sampleUniform(), lightPdf);
    const float lightDist = wi.length();
    wi = normalize(wi);

    if(lightPdf > 0 && !Li.isBlack()){
        const vec3 bsdfSpaceLightDir = worldToBsdf(wi, isect);
        const rgbColor f = bsdf.f(wo, bsdfSpaceLightDir);
        if(!f.isBlack()){
            ray shadowRay(p, wi);
            shadowRay.tMax = lightDist;
            if(!parent->intersect(shadowRay).hit){
                if(li.isPointSource()){
                    Ld += f * Li * fabs(dot(wi, isect.shadingNormal)) / lightPdf;
                }else{
                    bsdfPdf = bsdf.pdf(wo, bsdfSpaceLightDir);
                    const float weight = powerHeuristic(1, lightPdf, 1, bsdfPdf);
                    Ld += f * Li * fabs(dot(wi, isect.shadingNormal)) * weight / lightPdf;
                }
            }
        }
    }

    ///*
    if(!li.isPointSource()){
        bxdfType sampledType;
        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(), wo, wi, bxdfType(ALL & ~SPECULAR), sampledType, bsdfPdf);
        wi = normalize(bsdfToWorld(wi, isect));

        if(!f.isBlack() && bsdfPdf > 0.f){
            lightPdf = li.pdf();
            if(lightPdf > 0.f){
                ray lightRay(p, wi);
                const intersection isectL = li.intersect(lightRay);
                if(isectL.hit){
                    lightRay = ray(p, wi);
                    lightRay.tMax = isectL.t;
                    if(!parent->intersectB(lightRay)){
                        Li = li.L(lightRay);

                        if(!Li.isBlack()){
                            const float weight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
                            Ld += f * Li * fabs(dot(wi, isect.shadingNormal)) * weight / bsdfPdf;
                        }
                    }
                }
            }
        }
    }
    //*/

    return Ld;
}
