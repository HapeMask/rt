#include "tracer.hpp"
#include "acceleration/intersection.hpp"

const rgbColor whittedRayTracer::L(ray& r) const{
	const intersection i = parent->intersect1(r);
	if(i.hit){
		return i.s->getMaterial()->sample(r.origin, vec3(0,0,0), -r.direction);
	}else{
		return rgbColor(0,0,0);
	}
}
