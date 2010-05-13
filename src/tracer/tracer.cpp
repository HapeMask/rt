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
	if(!isect.hit){
		return rgbColor(0.2,0.3,1.f);
	}

    // Handle emissive and specular materials.
    const vec3& normal = isect.shadingNormal;
    const bsdf& b = isect.s->getMaterial()->getBsdf();
    const material& mat = *isect.s->getMaterial().get();
    rgbColor Li(0.f);

	if(isect.s->getMaterial()->isEmissive()){
		return clamp(mat.Le());
	}

    // Diffuse calculations.
	for(unsigned int i=0; i<parent->numLights(); ++i){
        const lightPtr li = parent->getLight(i);
		const point3& lightPosition = li->getPosition();
		const float lightDist = (lightPosition - r.origin).length();
        const vec3 lightDir =  normalize(lightPosition - r.origin);

		// Test for shadowing early.
		ray shadowRay(r.origin, lightDir);
		shadowRay.tMax = lightDist;
		const intersection isect2 = parent->intersect(shadowRay);
		if(isect2.hit){
            continue;
		}

        const rgbColor c = mat.sample(r.origin, -r.direction, lightDir, bxdfType(DIFFUSE | REFLECTION));;
		Li += (c * dot(normal, lightDir) *
		(1.f / (lightDist*lightDist))) * li->getPower() * li->getColor();
	}

    // Area light.
    const unsigned int areaSamples = 16;

    const shape& emitter = *parent->getEmitter(0).get();
    const primitive& prim = *emitter.getPrimitives()[sampleRange(0, emitter.getPrimitives().size()-1)].get();
    float samples[areaSamples][2];
    getLDSamples2D(samples, areaSamples);
    for(unsigned int i=0; i<areaSamples; ++i){
        //vec3 eNorm;
        //const point3 emitterSample = emitter.sampleSurface(eNorm, samples[i][0], samples[i][1]);
        const point3 emitterSample = prim.sampleSurface(samples[i][0], samples[i][1]);
        const vec3 eNorm = prim.getNormal(emitterSample);

        const float emitterDist = (emitterSample - r.origin).length();
        const vec3 emitterDir = normalize(emitterSample - r.origin);
        ray emitRay(r.origin, emitterDir);
        emitRay.tMax = emitterDist - EPSILON;
        const intersection isectE = parent->intersect(emitRay);

        if(!isectE.hit && dot(eNorm, emitterDir) <= 0.f){
            const rgbColor c = mat.sample(r.origin, -r.direction, emitterDir, bxdfType(DIFFUSE | REFLECTION));;
            Li += (emitter.getMaterial()->Le() * (1.f / (emitterDist*emitterDist)) * c * dot(normal, emitterDir))
                / (float)areaSamples;
        }
    }

    // Trace specular rays.
    vec3 specDir;
    const rgbColor fr =
        b.sampleF(0, 0, worldToBsdf(-r.direction, normal, isect.dsdu, isect.dsdv), specDir, bxdfType(SPECULAR | REFLECTION));
    specDir = bsdfToWorld(specDir, normal, isect.dsdu, isect.dsdv);

    if(!fr.isBlack()){
        ray r2(r.origin, specDir);
        Li += fr * _L(r2, depth+1);// * abs(dot(specDir, normal));
    }

    const rgbColor ft =
        b.sampleF(0, 0, worldToBsdf(-r.direction, normal, isect.dsdu, isect.dsdv), specDir, bxdfType(SPECULAR | TRANSMISSION));
    specDir = bsdfToWorld(specDir, normal, isect.dsdu, isect.dsdv);

    if(!ft.isBlack()){
        ray r2(r.origin, specDir);
        Li += ft * _L(r2, depth+1);// * abs(dot(specDir, normal));
    }

    return clamp(Li);
}
