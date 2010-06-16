#include "tracer.hpp"
#include "utility.hpp"

#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "geometry/triangle.hpp"
#include "materials/bsdf.hpp"

#include <omp.h>

const rgbColor whittedRayTracer::L(const ray& r) const{
    ray r2(r);
	return _L(r2);
}

const unsigned int nsamp = 512*512*16*2;
const rgbColor whittedRayTracer::_L(ray& r, const unsigned int& depth) const{
	if(depth > MAXDEPTH){
		return 0.f;
	}

	const intersection isect = parent->intersect(r);
    //return rgbColor((float)isect.debugInfo / 300.f, 0.f,0.f);

	if(!isect.hit){
        return 0.f;
	}else if(isect.li != NULL){
        return isect.li->L(r);
    }

    // Handle emissive and specular materials.
	const material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
	const vec3& normal = isect.shadingNormal;
	const bsdf& bsdf = mat.getBsdf();
    const vec3 wo = worldToBsdf(-r.direction, isect);

	bxdfType sampledType;
    rgbColor L(0.f);

	if(isect.s->getMaterial()->isEmissive()){
		return clamp(mat.Le());
	}

    // Diffuse calculations.
	float lightPdf = 0.f;
	for(unsigned int i=0; i<parent->numLights(); ++i){
        const lightPtr li = parent->getLight(i);
        if(li->isPointSource()){
            vec3 lightDir;
            const rgbColor Li = li->sampleL(r.origin, lightDir, sampleUniform(), sampleUniform(), lightPdf);
            const float lightDist = lightDir.length();
            lightDir = normalize(lightDir);


            // Test for shadowing early.
            ray shadowRay(r.origin, lightDir);
            shadowRay.tMax = lightDist;
            const intersection isect2 = parent->intersect(shadowRay);
            if(isect2.hit){
                continue;
            }

			const vec3 wi = worldToBsdf(lightDir, isect);
			const rgbColor f = bsdf.f(wo, wi, bxdfType(DIFFUSE | GLOSSY | REFLECTION)) + mat.Le();
            L += f * dot(normal, lightDir) * (Li / lightPdf);
        }else{
            // Area Sampling
            rgbColor areaContrib(0.f);

            for(unsigned int i=0; i<areaSamples; ++i){
                vec3 lightDir;

                const rgbColor Li = li->sampleL(r.origin, lightDir, sampleUniform(), sampleUniform(), lightPdf);

                ray shadowRay(r.origin, normalize(lightDir));
                shadowRay.tMax = lightDir.length();
                const intersection isect2 = parent->intersect(shadowRay);
                if(isect2.hit){
                    continue;
                }

                lightDir = normalize(lightDir);
				const vec3 wi = worldToBsdf(lightDir, isect);

				const rgbColor f = bsdf.f(wo, wi, bxdfType(DIFFUSE | GLOSSY | REFLECTION)) + mat.Le();
                areaContrib += f * dot(normal, lightDir) * (Li / lightPdf);
            }

            L += areaContrib / (float)areaSamples;
        }
	}

    // Trace specular rays.
    vec3 specDir;
	float pdf;
    const rgbColor fr =
        bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(), wo, specDir, bxdfType(SPECULAR | REFLECTION), sampledType, pdf);

    if(!fr.isBlack()){
		specDir = bsdfToWorld(specDir, isect);
        ray r2(r.origin, specDir);
        L += (fr / pdf) * _L(r2, depth+1) * fabs(dot(specDir, normal));
    }

    const rgbColor ft =
        bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(), wo, specDir, bxdfType(SPECULAR | TRANSMISSION), sampledType, pdf);

    if(!ft.isBlack()){
		specDir = bsdfToWorld(specDir, isect);
        ray r2(r.origin, specDir);
        L += (ft / pdf) * _L(r2, depth+1) * fabs(dot(specDir, normal));
    }

    return clamp(L);
}
