#include <vector>

#include "defaultaccelerator.hpp"
#include "geometry/primitive.hpp"

using std::shared_ptr;
using std::vector;

const intersection defaultAccelerator::intersect(ray& r) const{
	// Just check every shape we have.
	vector<intersection> hits;

	const point3 ro(r.origin);
	for(int i=0; i<shapes.size(); ++i){
        ray rCopy(r);
        const intersection isect = shapes[i]->intersect(rCopy);
		if(isect.hit){
			hits.push_back(isect);
		}
	}

	if(hits.size() == 0){
		return intersection(false);
	}

	// Grab the closest hit.
	float minDist = POS_INF;
	intersection closestIntersection;
	for(int i=0; i<hits.size(); ++i){
		if(hits[i].t < minDist){
			minDist = hits[i].t;
			closestIntersection = hits[i];
		}
	}

	r.origin += closestIntersection.t * r.direction;
	return closestIntersection;
}

const bool defaultAccelerator::intersectB(const ray& r) const{
	ray r2(r);
	for(size_t i=0; i<shapes.size(); ++i){
		if(shapes[i]->intersect(r2).hit){
			return true;
		}
	}

	return false;
}

void defaultAccelerator::build(const scene& s){
	for(size_t i = 0; i<s.getShapes().size(); ++i){
		shapePtr shape = s.getShapes()[i];
		for(size_t j = 0; j<shape->getPrimitives().size(); ++j){
			shapes.push_back(shape->getPrimitives()[j]);
		}
	}
}
