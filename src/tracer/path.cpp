#include <cmath>

#include "tracer.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"
#include "scene/scene.hpp"

rgbColor pathTracer::L(const ray& r) const {
    ray r2(r);
    return _L<true>(r2);
}

template <const bool recursiveSpecular>
rgbColor pathTracer::_L(ray& r, const int depth) const {
    rgbColor throughput(1.f), colorSum(0.f);
    bool lastBounceWasSpecular = false;

    for(int pathLength = 0; ; ++pathLength){
        // Copy the ray since we need the original for the light test below and
        // scene::intersect() modifies it.
        const ray rOrig(r);
        const intersection isect = parent.intersect(r);
        //return rgbColor(0, isect.debugInfo / 1e5f, 0);

        // If the ray hits a light directly (only possible for physical lights
        // (area, sphere etc...)), or if it misses everything, the light
        // contribution needs special handling.
        if(isect.li || !isect.hit){
            if(pathLength == 0 || lastBounceWasSpecular){
                for(int i=0; i<parent.numLights(); ++i){
                    const vec3 lightDir = parent.getLight(i).getPosition() - r.origin;
                    const float lightDist = norm(lightDir);

                    ray lightRay(r.origin, normalize(lightDir), EPSILON, lightDist);

                    const intersection isectL = parent.getLight(i).intersect(rOrig);
                    if(isectL.hit && !parent.intersectB(lightRay)){
                        colorSum += throughput * isectL.li->L(rOrig);
                    }
                }
            }

            if(!isect.li) {
                // TODO: Sky goes here.
                //colorSum += throughput * rgbColor(0.02f, 0.01f, 3.f);
            }

            break;
        }

        material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
        const vec3& normal = isect.shadingNormal;
        const vec3 wo = worldToBsdf(-r.direction, isect);
        const bsdf& bsdf = mat.getBsdf(isect.uv);

        // Perform explicit direct lighting computations.
        //
        // This is why the above test for specular bounces is needed.  If a ray
        // hits a light after bouncing from a specular object, only then do we
        // need to add in the light contribution. Explicit direct lighting
        // handles non-specular surfaces.

        //colorSum += throughput * (sampleOneLight(r.origin, wo, isect, bsdf) + mat.Le());
        colorSum += throughput * (sampleAllLights(r.origin, wo, isect, bsdf) + mat.Le());

        vec3 wi;
        float pdf = 0.f;

        bxdfType sampledType;

        // Only sample non-specular reflection on the first bounce, leave the
        // 1st-bounce specular hits for the recursive step below.
        const bxdfType reflectionType = (recursiveSpecular && pathLength == 0) ?
            bxdfType(ALL & ~SPECULAR) : ALL;

        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(),
                wo, wi, reflectionType, sampledType, pdf);

        if(f.isBlack() && pdf == 0.f){
            // Trace both specular reflection and transmission recursively.
            if(recursiveSpecular && pathLength == 0 && depth < MAX_DEPTH){
                vec3 specDir;

                const rgbColor fr =
                    bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(),
                            wo, specDir, bxdfType(SPECULAR | REFLECTION), sampledType, pdf);

                if(!fr.isBlack()){
                    specDir = bsdfToWorld(specDir, isect);
                    ray r2(r.origin, specDir);
                    colorSum += fr * _L<true>(r2, depth+1) * abs(dot(specDir, normal)) / pdf;
                }

                const rgbColor ft =
                    bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(),
                            wo, specDir, bxdfType(SPECULAR | TRANSMISSION), sampledType, pdf);

                if(!ft.isBlack()){
                    specDir = bsdfToWorld(specDir, isect);
                    ray r2(r.origin, specDir);
                    colorSum += ft * _L<true>(r2, depth+1) * abs(dot(specDir, normal)) / pdf;
                }
            }
            break;
        }

        wi = normalize(bsdfToWorld(wi, isect));

        throughput *= f * abs(dot(wi, normal)) / pdf;
        lastBounceWasSpecular = (sampledType & SPECULAR) != 0;

        // Russian Roulette: Paths that carry a lot of light will last longer
        // than darker paths.
        if(pathLength > rrThreshold){
            if(sampleUniform() > throughput.gray()){
                break;
            }

            throughput /= throughput.gray();
        }

        r.direction = wi;
        r.invDir = 1.f/wi;
        r.tMax = MAX_FLOAT;
        r.tMin = EPSILON;
    }

    if(!isFinite(colorSum.avg())) {
        return ERROR_COLOR;
    } else {
        return colorSum;
    }
}
