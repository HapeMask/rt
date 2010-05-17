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
        const intersection isect = parent->intersect(r);
        if(!isect.hit){
            break;
        }

        const material& mat = *isect.s->getMaterial().get();
        if(pathLength == 0 || lastBounceWasSpecular){
            L += mat.Le() * throughput;
            if(parent->getLight(0)->intersect(r)){
                L += parent->getLight(0)->L(r.origin);
            }
        }

        const vec3& normal = isect.shadingNormal;
        const bsdf& bsdf = mat.getBsdf();
        const vec3 wo = worldToBsdf(-r.direction, isect.shadingNormal, isect.dpdu, isect.dpdv);

        L += throughput * sampleOneLight(r.origin, wo, isect, bsdf);

        vec3 wi;
        float pdf = 0.f;

        const rgbColor f = bsdf.sampleF(0,0,0,wo, wi, ALL, pdf);
        if(f.isBlack() || pdf == 0.f){
            break;
        }

        wi = normalize(bsdfToWorld(wi, normal, isect.dpdu, isect.dpdv));
        throughput *= f * abs(dot(wi, normal)) / pdf;

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
    }

    return clamp(L);
}

const rgbColor pathTracer::sampleOneLight(const point3& p, const vec3& wo, const intersection& isect,
        const bsdf& bsdf) const{
    return sampleDirect(p, wo, isect, bsdf, *parent->getLight(0).get());
}

const rgbColor pathTracer::sampleDirect(const point3& p, const vec3& wo,
        const intersection& isect, const bsdf& bsdf, const light& li) const {
    vec3 wi;
    float bsdfPdf, lightPdf;
    rgbColor Ld(0.f);

    float sample[2];
    getNextSample(sample);

    rgbColor Li = li.sampleL(p, wi, sample[0], sample[1], lightPdf);
    //rgbColor Li = li.sampleL(p, wi, 0, 0, lightPdf);
    const float lightDist = wi.length();
    wi = normalize(wi);

    if(lightPdf > 0 && !Li.isBlack()){
        const vec3 bsdfSpaceLightDir = worldToBsdf(wi, isect.shadingNormal, isect.dpdu, isect.dpdv);
        const rgbColor f = bsdf.f(wo, bsdfSpaceLightDir);
        if(!f.isBlack()){
            ray shadowRay(p, wi);
            shadowRay.tMax = lightDist;
            const intersection isect2 = parent->intersect(shadowRay);
            if(!isect2.hit){
                if(li.isPointSource()){
                    Ld += f * Li * abs(dot(wi, isect.shadingNormal)) / lightPdf;
                }else{
                    bsdfPdf = bsdf.pdf(wo, bsdfSpaceLightDir);
                    const float weight = powerHeuristic(1, lightPdf, 1, bsdfPdf);
                    Ld += f * Li * abs(dot(wi, isect.shadingNormal)) * weight / lightPdf;
                }
            }
        }
    }

    if(!li.isPointSource()){
        const rgbColor f = bsdf.sampleF(0,0,0, wo, wi, bxdfType(ALL & ~SPECULAR), bsdfPdf);
        wi = normalize(bsdfToWorld(wi, isect.shadingNormal, isect.dpdu, isect.dpdv));

        if(!f.isBlack() && bsdfPdf > 0.f){
            lightPdf = li.pdf();
            if(lightPdf > 0.f){
                const float weight = powerHeuristic(1, bsdfPdf, 1, lightPdf);
                if(li.intersect(ray(p, wi))){
                    Li = li.L(p);

                    if(!Li.isBlack()){
                        Ld += f * Li * abs(dot(wi, isect.shadingNormal)) * weight / bsdfPdf;
                    }
                }
            }
        }
    }

    return Ld;
}
