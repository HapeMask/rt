#include "tracer.hpp"
#include "utility.hpp"

#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "geometry/triangle.hpp"
#include "materials/bsdf.hpp"

const rgbColor whittedRayTracer::L(const ray& r) const{
    ray r2(r);
	return _L(r2);
}

const rgbColor whittedRayTracer::_L(ray& r, const int& depth) const{
	if(depth > MAXDEPTH){
		return 0.f;
	}

	const intersection isect = parent->intersect(r);
    //return rgbColor((float)isect.debugInfo / 300.f, 0.f,0.f);

	if(!isect.hit){
		return rgbColor(0.2,0.3,1.f);
	}

    // Handle emissive and specular materials.
    const vec3& normal = isect.shadingNormal;
    const bsdf& b = isect.s->getMaterial()->getBsdf();
    const material& mat = *isect.s->getMaterial().get();
    rgbColor L(0.f);

	if(isect.s->getMaterial()->isEmissive()){
		return clamp(mat.Le());
	}

    // Diffuse calculations.
	for(unsigned int i=0; i<parent->numLights(); ++i){
        const lightPtr li = parent->getLight(i);
        if(li->isPointSource()){
            vec3 lightDir;
            float pdf;
            const rgbColor Li = li->sampleL(r.origin, lightDir, 0.f, 0.f, pdf);
            const float lightDist = lightDir.length();
            lightDir = normalize(lightDir);


            // Test for shadowing early.
            ray shadowRay(r.origin, lightDir);
            shadowRay.tMax = lightDist;
            const intersection isect2 = parent->intersect(shadowRay);
            if(isect2.hit){
                continue;
            }

            const rgbColor c = mat.sample(r.origin, -r.direction, lightDir, bxdfType(DIFFUSE | REFLECTION));;
            L += c * dot(normal, lightDir) * Li;
        }else{
            // Area Sampling
            const unsigned int areaSamples = 16;
            float samples[areaSamples][2];
            getLDSamples2D(samples, areaSamples);

            rgbColor areaContrib(0.f);
            for(unsigned int i=0; i<areaSamples; ++i){
                vec3 lightDir;
                float pdf;
                const rgbColor Li = li->sampleL(r.origin, lightDir, samples[i][0], samples[i][1], pdf);

                ray shadowRay(r.origin, normalize(lightDir));
                shadowRay.tMax = lightDir.length();
                const intersection isect2 = parent->intersect(shadowRay);
                if(isect2.hit){
                    continue;
                }

                lightDir = normalize(lightDir);
                const rgbColor c = mat.sample(r.origin, -r.direction, lightDir, bxdfType(DIFFUSE | REFLECTION));;
                areaContrib += ((Li * dot(normal, lightDir)) / pdf) * c;
            }

            L += areaContrib / (float)areaSamples;
        }
	}

    // Area Sampling
    /*
    const unsigned int areaSamples = 4;
    float samples[areaSamples][2];
    getLDSamples2D(samples, areaSamples);

    rgbColor areaContrib(0.f);
    for(unsigned int i=0; i<areaSamples; ++i){
        vec3 areaDir;
        float pdf;
        const rgbColor Li = 
    }
    */

    // Trace specular rays.
    vec3 specDir;
    const rgbColor fr =
        b.sampleF(0, 0, 0, worldToBsdf(-r.direction, normal, isect.dsdu, isect.dsdv), specDir, bxdfType(SPECULAR | REFLECTION));
    specDir = bsdfToWorld(specDir, normal, isect.dsdu, isect.dsdv);

    if(!fr.isBlack()){
        ray r2(r.origin, specDir);
        L += fr * _L(r2, depth+1);;
    }

    const rgbColor ft =
        b.sampleF(0, 0, 0, worldToBsdf(-r.direction, normal, isect.dsdu, isect.dsdv), specDir, bxdfType(SPECULAR | TRANSMISSION));
    specDir = bsdfToWorld(specDir, normal, isect.dsdu, isect.dsdv);

    if(!ft.isBlack()){
        ray r2(r.origin, specDir);
        L += ft * _L(r2, depth+1);
    }

    return clamp(L);
}
