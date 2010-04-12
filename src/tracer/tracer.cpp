#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "utility.hpp"

const rgbColor whittedRayTracer::L(ray& r) const{
	const intersection isect = parent->intersect(r);
	if(!isect.hit){
		return rgbColor(0,0,0);
	}

	if(isect.s->getMaterial()->isEmissive()){
		return isect.s->getMaterial()->sampleL();
	}

	rgbColor c = isect.s->getMaterial()->sample(r.origin, vec3(0,0,0), -r.direction);;
	if(parent->getLights().size() > 0){
		const point3 lightPosition = parent->getLights()[0]->getPosition();
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
		c *= 1.f / (lightPosition - r.origin).length2();
		c *= parent->getLights()[0]->getPower();

		return clamp(c);
	}else{
		cerr << "NO LIGHTS" << endl;
		return rgbColor(0,0,0);
	}
}
