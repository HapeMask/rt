#include "tracer.hpp"
#include "utility.hpp"

#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "geometry/triangle.hpp"
#include "materials/bsdf.hpp"
#include "scene/scene.hpp"

#include <omp.h>

const int areaSamples = 4;

const rgbColor whittedRayTracer::L(const ray& r) const{
    ray r2(r);
	return _L(r2);
}

const int nsamp = 512*512*16*2;
const rgbColor whittedRayTracer::_L(ray& r, const int& depth) const{
	if(depth > MAX_DEPTH){
		return rgbColor(0.f);
	}

	const intersection isect = parent.intersect(r);
    //return rgbColor(0, isect.debugInfo / 1e8, 0);

	if(!isect.hit){
        return rgbColor(0.f);
	}else if(isect.li != NULL){
        return isect.li->L(r);
    }

    // Handle emissive and specular materials.
	material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
	const vec3& normal = isect.shadingNormal;
	const bsdf& bsdf = mat.getBsdf(isect.uv);
    const vec3 wo = worldToBsdf(-r.direction, isect);

	bxdfType sampledType;
    rgbColor L(0.f);

	if(isect.s->getMaterial()->isEmissive()){
		return clamp(mat.Le());
	}

    // Diffuse calculations.
	float lightPdf = 0.f;
	for(int i=0; i<parent.numLights(); ++i){
        const lightPtr li = parent.getLight(i);
        if(li->isPointSource()){
            vec3 lightDir;
            const rgbColor Li = li->sampleL(r.origin, lightDir, sampleUniform(), sampleUniform(), lightPdf);
            const float lightDist = lightDir.length();
            lightDir = normalize(lightDir);

            // Test for shadowing early.
            ray shadowRay(r.origin, lightDir);
            shadowRay.tMax = lightDist;
			if(!parent.intersectB(shadowRay)){
				const vec3 wi = worldToBsdf(lightDir, isect);
				const rgbColor f = bsdf.f(wo, wi, bxdfType(DIFFUSE | GLOSSY | REFLECTION)) + mat.Le();
				L += f * dot(normal, lightDir) * (Li / lightPdf);
			}
        }else{
            rgbColor areaContrib(0.f);

            for(int i=0; i<areaSamples; ++i){
                vec3 lightDir;

                const rgbColor Li = li->sampleL(r.origin, lightDir, sampleUniform(), sampleUniform(), lightPdf);

                ray shadowRay(r.origin, normalize(lightDir));
                shadowRay.tMax = lightDir.length() + EPSILON;

				if(!parent.intersectB(shadowRay) && li->intersect(shadowRay).hit){
					lightDir = normalize(lightDir);
					const vec3 wi = worldToBsdf(lightDir, isect);

					const rgbColor f = bsdf.f(wo, wi, bxdfType(DIFFUSE | GLOSSY | REFLECTION)) + mat.Le();
					areaContrib += f * dot(normal, lightDir) * (Li / lightPdf);
				}
            }
            L += areaContrib / (float)areaSamples;
        }
	}

    // Trace specular rays.
    vec3 specDir;
	float pdf;
    const rgbColor fr =
        bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(),
                wo, specDir, bxdfType(GLOSSY | SPECULAR | REFLECTION), sampledType, pdf);

    if(!fr.isBlack()){
		specDir = bsdfToWorld(specDir, isect);
        ray r2(r.origin, specDir);
        L += (fr / pdf) * _L(r2, depth+1) * fabsf(dot(specDir, normal));
    }

    const rgbColor ft =
        bsdf.sampleF(sampleUniform(), sampleUniform(), sampleUniform(),
                wo, specDir, bxdfType(GLOSSY | SPECULAR | TRANSMISSION), sampledType, pdf);

    if(!ft.isBlack()){
		specDir = bsdfToWorld(specDir, isect);
        ray r2(r.origin, specDir);
        L += (ft / pdf) * _L(r2, depth+1) * fabsf(dot(specDir, normal));
    }

    // TODO: Really need to find out what's causing this.
    if(L.r < 0.f || L.g < 0.f || L.b < 0.f ||
            isnan(L.r) || isnan(L.g) || isnan(L.b))
    {
        return rgbColor(0.f);
    }else{
        return L;
    }
}
