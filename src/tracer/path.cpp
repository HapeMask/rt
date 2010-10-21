#include "tracer.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"
#include "scene/scene.hpp"

const rgbColor pathTracer::L(const ray& r) const {
    ray r2(r);
    return _L<true>(r2);
}

template <const bool recursiveSpecular>
const rgbColor pathTracer::_L(ray& r, const int depth) const {
    rgbColor throughput(1.f), L(0.f);
    bool lastBounceWasSpecular = false;

    for(int pathLength = 0; ; ++pathLength){
        // Copy the ray since we need the original for the light test below and
        // scene::intersect() modifies it.
        const ray rOrig(r);
        const intersection isect = parent.intersect(r);
        //return rgbColor(0, isect.debugInfo / 2400.f, 0);

        if(!isect.hit || isect.li){
            if(pathLength == 0 || lastBounceWasSpecular){
                for(size_t i=0; i<parent.numLights(); ++i){
                    const float lightDist = (parent.getLight(i)->getPosition() - rOrig.origin).length();
                    ray lightRay(rOrig, EPSILON, lightDist);

                    const intersection isectL = parent.getLight(i)->intersect(rOrig);
                    if(isectL.hit && !parent.intersectB(lightRay)){
                        L += throughput * isectL.li->L(rOrig);
                    }
                }

                // If the first hit was a light, don't keep bouncing.
                if(pathLength == 0){
                    return L;
                }
            }

            //L += throughput * rgbColor(0.02f, 0.01f, 0.03f);

            break;
        }

		// Handle direct light bounces from a specular surface (also if a ray
		// hits a light source on the first bounce).
        if((pathLength == 0 || lastBounceWasSpecular) && isect.li){
            L += throughput * isect.li->L(rOrig);
        }

        const material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
        const vec3& normal = isect.shadingNormal;
        const bsdf& bsdf = mat.getBsdf();
        const vec3 wo = worldToBsdf(-r.direction, isect);

		// Perform explicit direct lighting computations.
		//
        // This is why the above test for specular bounces is needed.  If a ray
        // hits a light after bouncing from a specular object, only then do we
        // need to add in the light contribution. Explicit direct lighting
        // handles non-specular surfaces.

        //L += throughput * (sampleOneLight(r.origin, wo, isect, bsdf) + mat.Le());
        L += throughput * (sampleAllLights(r.origin, wo, isect, bsdf) + mat.Le());

        vec3 wi;
        float pdf = 0.f;

        bxdfType sampledType;

		// Only sample non-specular reflection on the first bounce, leave the
		// 1st-bounce specular hits for the recursive step below.
		const bxdfType reflectionType = (recursiveSpecular && pathLength == 0) ? bxdfType(ALL & ~SPECULAR) : ALL;
        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(), wo, wi, reflectionType, sampledType, pdf);
        if(f.isBlack() && pdf != 0){
            return rgbColor(0.f);
        }

        if(f.isBlack()){
			// Trace both specular reflection and refraction recursively.
			if(recursiveSpecular && pathLength == 0 && depth < MAX_DEPTH){
				vec3 specDir;

				const rgbColor fr =
					bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(), wo, specDir, bxdfType(SPECULAR | REFLECTION), sampledType, pdf);

				if(!fr.isBlack()){
					specDir = bsdfToWorld(specDir, isect);
					ray r2(r.origin, specDir);
					L += fr * _L<true>(r2, depth+1) * fabs(dot(specDir, normal)) / pdf;
				}

				const rgbColor ft =
					bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(), wo, specDir, bxdfType(SPECULAR | TRANSMISSION), sampledType, pdf);

				if(!ft.isBlack()){
					specDir = bsdfToWorld(specDir, isect);
					ray r2(r.origin, specDir);
					L += ft * _L<true>(r2, depth+1) * fabs(dot(specDir, normal)) / pdf;
				}
			}
			break;
        }

        wi = normalize(bsdfToWorld(wi, isect));

        throughput *= f * fabs(dot(wi, normal)) / pdf;
        lastBounceWasSpecular = (sampledType & SPECULAR) != 0;

        if(pathLength > 4){
            if(sampleUniform() > pathContinueProbability){
                break;
            }

            throughput /= pathContinueProbability;
        }

        r.direction = wi;
        r.invDir = 1.f/wi;
        r.tMax = MAX_FLOAT;
        r.tMin = EPSILON;
    }

    return L;
}
