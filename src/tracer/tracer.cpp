#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "utility.hpp"
#include "geometry/triangle.hpp"

const rgbColor whittedRayTracer::L(const ray& r) const{
    ray r2(r);
	return _L(r2);
}

const rgbColor whittedRayTracer::_L(ray& r, const int& depth) const{
	if(depth > MAXDEPTH){
		return rgbColor(0,0,0);
	}

	const intersection isect = parent->intersect(r);
	if(!isect.hit){
		return rgbColor(0,0,0);
	}

    // Handle emissive and specular materials.
    const vec3& normal = isect.p->getNormal(r.origin);
	if(isect.s->getMaterial()->isEmissive()){
		return clamp(isect.s->getMaterial()->sampleL());
	}else if(isect.s->getMaterial()->isSpecular()){
        vec3 specDir;
        const rgbColor f =
            isect.s->getMaterial()->sampleF(0, 0, normal, -r.direction, specDir);

        // TIR
        if(f.r < 0){
            return rgbColor(1,0,0);
        }

        ray r2(r.origin, specDir);

		return _L(r2, depth+1);
	}

    // Diffuse stuff.
    rgbColor Li(0,0,0);
	const rgbColor c = isect.s->getMaterial()->sample(r.origin, vec3(0,0,0), -r.direction);;
	for(unsigned int i=0; i<parent->numLights(); ++i){
        const lightPtr li = parent->getLight(i);
		const point3 lightPosition = li->getPosition();
		const float lightDist = (lightPosition - r.origin).length();

		// Test for shadowing early.
		ray shadowRay(point3(r.origin), normalize(lightPosition - r.origin));
		shadowRay.tMax = lightDist;
		const intersection isect2 = parent->intersect(shadowRay);
		if(isect2.hit){
            continue;
		}

		Li += (c * dot(normal, normalize(lightPosition - r.origin)) *
		(1.f / (lightDist*lightDist))) * li->getPower() * li->getColor();
	}

    // Area lights.
    /*
    int hits = 0;
    for(unsigned int j=0; j<4; ++j){
        const point3 randPoint = parent->getAccelerator()->getEmitter(0)->uniformSampleSurface();
        ray lightRay(point3(r.origin), normalize(randPoint - r.origin));
        lightRay.tMax = (randPoint - r.origin).length();
        const intersection isectE = parent->intersectEB(lightRay);
        if(isectE.hit){
            Li += isectE.s->getMaterial()->sampleL() * c;
        }
    }
    */

    return clamp(Li);
}
