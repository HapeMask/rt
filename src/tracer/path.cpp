#include "path.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"

const rgbColor pathTracer::L(const ray& r) const {
    ray r2(r);
    return _L(r2);
}

const rgbColor pathTracer::_L(ray& r) const {
    rgbColor throughput = 1.f, L = 0.f;
    bool lastBounceWasSpecular = false;

    for(unsigned int pathLength = 0; ; ++pathLength){
        // Copy the ray since we need the original for the light test below and
        // scene::intersect() modifies it.
        const ray rOrig(r);
        const intersection isect = parent->intersect(r);
        //return rgbColor(0, isect.debugInfo / 2400.f, 0);

        if(!isect.hit){
            if(pathLength == 0 || lastBounceWasSpecular){
                for(unsigned int i=0; i<parent->numLights(); ++i){
                    const float lightDist = (parent->getLight(i)->getPosition() - rOrig.origin).length();
                    ray lightRay(rOrig, EPSILON, lightDist);

                    const intersection isectL = parent->getLight(i)->intersect(rOrig);
                    if(isectL.hit && !parent->intersectB(lightRay)){
                        L += throughput * isectL.li->L(rOrig);
                    }
                }
            }

            //L += throughput * rgbColor(0.f, 0.1f, 0.2f);
            break;
        }

        if((pathLength == 0 || lastBounceWasSpecular) && isect.li){
            L += throughput * isect.li->L(rOrig);
        }

        const material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
        const vec3& normal = isect.shadingNormal;
        const bsdf& bsdf = mat.getBsdf();
        const vec3 wo = worldToBsdf(-r.direction, isect);

        //L += throughput * (sampleOneLight(r.origin, wo, isect, bsdf) + mat.Le());
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

        if(pathLength > 4){
            const float continueProbability = 0.8f;
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
    return L;
}
