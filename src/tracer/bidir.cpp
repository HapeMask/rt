#include "bidir.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"

const rgbColor bdpt::L(const ray& r) const {
    ray eyeRay(r);
    vector<pathPoint> eyePath, lightPath;

    createPath(eyeRay, eyePath);

    // Pick a random light and start a path on it in a random direction.
    unsigned int i = sampleRange(sampleUniform(), 0, parent->numLights()-1);
    while(parent->getLight(i)->isPointSource()){
        i = sampleRange(sampleUniform(), 0, parent->numLights()-1);
    }

    // Sample the surface to find the start of the light path. 
    const point3 p = parent->getLight(i)->uniformSampleSurface();
    const vec3& normal = parent->getLight(i)->getNormal();

    vec3 wi;
    uniformSampleHemisphere(wi);

    // Make up a coordinate system just to convert the sampled direction from
    // light normal space -> world space.
    vec3 binormal, tangent;
    makeCoordinateSystem(normal, binormal, tangent);
    wi = normalize(bsdfToWorld(wi, normal, binormal, tangent));

    ray lightRay(p, wi);
    createPath(lightRay, lightPath);
    if(lightPath.size() < 2){
        return 0.f;
    }

    // Connect the paths with a visibility ray (if possible).
    if(!parent->intersectB(ray(eyePath.back().p, normalize(lightPath.back().p - eyePath.back().p)))){
        //return eyePath.back().L * lightPath.back().throughput + lightPath.back().L * eyePath.back().throughput;
        return (eyePath.back().L + lightPath.back().L) * (lightPath.back().throughput * eyePath.back().throughput);
    }else{
        return 0.f;
    }
}

void bdpt::createPath(ray& r, vector<pathPoint>& points) const {
    rgbColor throughput = 1.f, L = 0.f;
    bool lastBounceWasSpecular = false, addedPoint = false;
    unsigned int pathLength;

    for(pathLength = 0; ; ++pathLength){
        addedPoint = false;

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

        pathPoint pp = {r.origin, L, throughput};
        points.push_back(pp);
        addedPoint = true;

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

    // If the last ray didn't miss, but the loop broke out, then
    // add the last point.
    if(!addedPoint){
        pathPoint pp = {r.origin, L, throughput};
        points.push_back(pp);
    }
}
