#include <vector>
#include <tr1/memory>

#include "defaultaccelerator.hpp"
#include "geometry/primitive.hpp"

using namespace std;
using tr1::shared_ptr;

const intersection defaultAccelerator::intersect(ray& r) const{
	// Just check every shape we have.
	vector<intersection> hits;
	vector<point3> hitPoints;

	const point3 ro(r.origin);
	for(unsigned int i=0; i<shapes.size(); ++i){
		if(shapes[i]->intersect(r)){
			hits.push_back(intersection(shapes[i]->getParent(), shapes[i]));
			hitPoints.push_back(point3(r.origin));

			// Reset the ray's origin.
			r.origin = ro;
		}
	}

	if(hits.size() == 0){
		return intersection(false);
	}

	// Grab the closest hit.
	float minDist = 10000000.f;
	point3 closestPoint;
	intersection closestIntersection;
	for(unsigned int i=0; i<hitPoints.size(); ++i){
		const float dist = (hitPoints[i] - ro).length2();
		if(dist < minDist){
			minDist = dist;
			closestPoint = hitPoints[i];
			closestIntersection = hits[i];
		}
	}

	r.origin = closestPoint;
	return closestIntersection;
}

const intersection defaultAccelerator::intersectE(ray& r) const {
	// Just check every shape we have.
	vector<intersection> hits;
	vector<point3> hitPoints;

	const point3 ro(r.origin);
	for(unsigned int i=0; i<emitters.size(); ++i){
		if(emitters[i]->intersect(r)){
			hits.push_back(intersection(emitters[i]->getParent(), emitters[i]));
			hitPoints.push_back(point3(r.origin));

			// Reset the ray's origin.
			r.origin = ro;
		}
	}

	if(hits.size() == 0){
		return intersection(false);
	}

	// Grab the closest hit.
	float minDist = 10000000.f;
	point3 closestPoint;
	intersection closestIntersection;
	for(unsigned int i=0; i<hitPoints.size(); ++i){
		const float dist = (hitPoints[i] - ro).length2();
		if(dist < minDist){
			minDist = dist;
			closestPoint = hitPoints[i];
			closestIntersection = hits[i];
		}
	}

	r.origin = closestPoint;
	return closestIntersection;
}

const bool defaultAccelerator::intersectB(const ray& r) const{
	ray r2(r);
	for(size_t i=0; i<shapes.size(); ++i){
		if(shapes[i]->intersect(r2)){
			return true;
		}
	}

	return false;
}

const bool defaultAccelerator::intersectEB(const ray& r) const{
	ray r2(r);
	for(size_t i=0; i<emitters.size(); ++i){
		if(emitters[i]->intersect(r2)){
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

	for(size_t i = 0; i<s.getEmitters().size(); ++i){
		shapePtr emitter = s.getEmitters()[i];
		for(size_t j = 0; j<emitter->getPrimitives().size(); ++j){
			emitters.push_back(emitter->getPrimitives()[j]);
			shapes.push_back(emitter->getPrimitives()[j]);
		}
	}
}
