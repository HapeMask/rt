#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "utility.hpp"

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
		return isect.s->getMaterial()->sampleL();
	}else if(isect.s->getMaterial()->isSpecular()){
        const vec3 refractionNormal =
            (dot(r.direction, normal) < 0) ? normal : -normal;

        vec3 specDir;
        isect.s->getMaterial()->sampleF(0, 0, refractionNormal, -r.direction, specDir);
        ray r2(r.origin, specDir);

		return _L(r2, depth+1);
	}

    // Diffuse stuff.
    rgbColor ret(0,0,0);
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

		ret += (c * dot(normal, normalize(lightPosition - r.origin)) *
		(1.f / (lightDist*lightDist))) * li->getPower() * li->getColor();
	}

    return clamp(ret);
}
