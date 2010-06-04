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

            //L += throughput * rgbColor(0.f, 0.1f, 0.2f);
            break;
        }

        const material& mat = *isect.s->getMaterial().get();
        const vec3& normal = isect.shadingNormal;
        const bsdf& bsdf = mat.getBsdf();
        const vec3 wo = worldToBsdf(-r.direction, isect);

        //L += throughput * sampleOneLight(r.origin, wo, isect, bsdf);
        L += throughput * (sampleAllLights(r.origin, wo, isect, bsdf) + mat.Le());

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

const rgbColor pathTracer::sampleAllLights(const point3& p, const vec3& wo, const intersection& isect,
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

const rgbColor pathTracer::sampleDirect(const point3& p, const vec3& wo,
        const intersection& isect, const bsdf& bsdf, const light& li) const {
    vec3 wi;
    float bsdfPdf, lightPdf;
    rgbColor lightSample(0.f), bsdfSample(0.f);
    float lightWeight, bsdfWeight;

    //float sample[2];
    //getNextSample(sample);
    //rgbColor Li = li.sampleL(p, wi, sample[0], sample[1], lightPdf);

    // Sample the light to find a point on the surface and the emission
    // at that point.
    rgbColor Li = li.sampleL(p, wi, sampleUniform(), sampleUniform(), lightPdf);
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
                }else{
                    bsdfPdf = bsdf.pdf(wo, bsdfSpaceLightDir);

                    lightWeight = powerHeuristic(1, lightPdf, 1, bsdfPdf);
                    lightSample = f * Li * fabs(dot(wi, isect.shadingNormal)) / lightPdf;
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
                    Li = li.L(lightRay);

                    if(!Li.isBlack()){
                        bsdfWeight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
                        bsdfSample = f * Li * fabs(dot(wi, isect.shadingNormal)) / bsdfPdf;
                    }
                }
            }
        }
    }

    // Combine the samples based on the weight from the chosen heuristic.
    return lightWeight * lightSample + bsdfWeight * bsdfSample;
}
