#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "utility.hpp"

const rgbColor whittedRayTracer::L(ray& r) const{
	return _L(r);
}

const rgbColor whittedRayTracer::_L(ray& r, const int& depth) const{
	if(depth > MAXDEPTH){
		return rgbColor(0,0,0);
	}

	const intersection isect = parent->intersect(r);
	if(!isect.hit){
		return rgbColor(0,0,0);
	}

	if(isect.s->getMaterial()->isEmissive()){
		return isect.s->getMaterial()->sampleL();
	}else if(isect.s->getMaterial()->isSpecular()){
		ray r2 = ray(r.origin, reflect(r.direction, isect.p->getNormal(r.origin)));
		return _L(r2, depth+1);
	}

	rgbColor c = isect.s->getMaterial()->sample(r.origin, vec3(0,0,0), -r.direction);;
	if(parent->numLights() > 0){
		const point3 lightPosition = parent->getLight(0)->getPosition();
		const float lightDist = (lightPosition - r.origin).length();

		// Test for shadowing early.
		ray shadowRay(point3(r.origin), normalize(lightPosition - r.origin));
		shadowRay.tMax = lightDist;
		const intersection isect2 = parent->intersect(shadowRay);
		if(isect2.hit){
			return rgbColor(0,0,0);
		}

		c *= clamp(dot(isect.p->getNormal(r.origin),
				normalize(lightPosition - r.origin)));
		c *= 1.f / (lightDist*lightDist);
		c *= parent->getLight(0)->getPower();
		c *= parent->getLight(0)->getColor();

		return clamp(c);
	}else{
		cerr << "NO LIGHTS" << endl;
		return rgbColor(0,0,0);
	}
}
