#include "tracer.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"

const rgbColor whittedRayTracer::L(ray& r) const{
	const intersection isect = parent->intersect1(r);
	if(isect.hit){
		rgbColor c = isect.s->getMaterial()->sample(r.origin, vec3(0,0,0), -r.direction);;
		if(parent->getLights().size() > 0){
			point3 lightPosition = parent->getLights()[0]->getPosition();

			c *= dot(isect.p->getNormal(),
					normalize(lightPosition - r.origin));
			//c *= 1.f / (lightPosition - r.origin).length2();

			// Test for shadowing.
			const vec3 offset = EPSILON * isect.p->getNormal();
			ray shadowRay(r.origin + offset, normalize(lightPosition - r.origin));
			if(parent->intersectB(shadowRay)){
				return rgbColor(0,0,0);
			}
		}

		return c;
	}else{
		return rgbColor(0,0,0);
	}
}
