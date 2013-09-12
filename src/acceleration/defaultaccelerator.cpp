#include <vector>

#include "defaultaccelerator.hpp"
#include "geometry/primitive.hpp"

using std::shared_ptr;
using std::vector;

const intersection defaultAccelerator::intersect(ray& r) const{
	vector<intersection> hits;

	const point3 ro(r.origin);
	for(auto prim : primitives){
        ray rCopy(r);
        const intersection isect = prim->intersect(rCopy);
		if(isect.hit){
			hits.push_back(isect);
		}
	}

    if(hits.empty()){ return noIntersect; }

	// Grab the closest hit.
	float minDist = POS_INF;
	intersection closestIntersection;
	for(auto hit : hits){
		if(hit.t < minDist){
			minDist = hit.t;
			closestIntersection = hit;
		}
	}

	r.origin += closestIntersection.t * r.direction;
	return closestIntersection;
}

bool defaultAccelerator::intersectB(const ray& r) const{
	for(auto prim : primitives){
        if(prim->intersectB(r)){ return true; }
	}

	return false;
}

void defaultAccelerator::build(const scene& s){
	for(const auto& shape : s.getShapes()){
		for(const auto& prim : shape->getPrimitives()){
			primitives.push_back(prim.get());
		}
	}
}
