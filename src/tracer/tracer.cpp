#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

const rgbColor whittedRayTracer::L(ray& r) const{
	const intersection isect = parent->intersect(r);
	if(!isect.hit){
		return rgbColor(0,0,0);
	}


	rgbColor c = isect.s->getMaterial()->sample(r.origin, vec3(0,0,0), -r.direction);;
	if(parent->getLights().size() > 0){
		point3 lightPosition = parent->getLights()[0]->getPosition();

		c *= dot(isect.p->getNormal(r.origin),
				normalize(lightPosition - r.origin));
		//c *= 1.f / (lightPosition - r.origin).length2();

		// Test for shadowing.
		if(!isect.s->getMaterial()->isEmissive()){
			ray shadowRay(r.origin, normalize(lightPosition - r.origin));
			if(parent->intersectB(shadowRay)){
				c = rgbColor(0,0,0);
			}
		}else{
			return isect.s->getMaterial()->sampleL();
		}

		// Sample emitters.

		/*
		rgbColor indir;
		vec3 w;
		for(int i=0; i<32; i++){
			sampleHemisphere(w, sampleUniform(), sampleUniform());
			ray r2(r.origin, w);
			const intersection isectE = parent->intersect(r2);
			if(isectE.hit){
				indir +=
						isectE.s->getMaterial()->
						sample(point3(0,0,0), vec3(0,0,0), vec3(0,0,0));
			}
		}
		indir /= 32.f;
		cerr << "ind: " << indir << endl;
		*/

		return c;

	}else{
		return rgbColor(0,0,0);
	}
}
