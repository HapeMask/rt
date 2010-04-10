#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

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
		point3 lightPosition = parent->getLights()[0]->getPosition();

		c *= dot(isect.p->getNormal(r.origin),
				normalize(lightPosition - r.origin));
		c *= 1.f / (lightPosition - r.origin).length2();
		c *= parent->getLights()[0]->getPower() ;

		// Test for shadowing.
		/*
		const ray shadowRay(r.origin, normalize(lightPosition - r.origin));
		if(parent->intersectB(shadowRay) || parent->intersectEB(shadowRay)){
			c = rgbColor(0,0,0);
		}
		*/

		return c;
	}else{
		cerr << "NO LIGHTS" << endl;
		return rgbColor(0,0,0);
	}
}
