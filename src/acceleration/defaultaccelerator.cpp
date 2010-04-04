#include <vector>
#include "defaultaccelerator.hpp"
#include "geometry/primitive.hpp"

using namespace std;

const intersection defaultAccelerator::intersect(ray& r) const{
	// Just check every shape we have.
	vector<intersection> hits;
	vector<point3> hitPoints;

	const point3 ro(r.origin);
	for(unsigned int i=0; i<contents_.size(); i++){
		if(contents_[i]->intersect(r)){
			hits.push_back(intersection(contents_[i]->parent(), contents_[i]));
			hitPoints.push_back(r.origin);

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
	for(unsigned int i=0; i<hitPoints.size(); i++){
		const float dist = (r.origin - hitPoints[i]).length2();
		if(dist < minDist){
			minDist = dist;
			closestPoint = hitPoints[i];
			closestIntersection = hits[i];
		}
	}

	r.origin = closestPoint;
	return closestIntersection;
}

const intersection defaultAccelerator::intersect1(ray& r) const{
	// Just check every shape we have.
	for(unsigned int i=0; i<contents_.size(); i++){
		if(contents_[i]->intersect(r)){
			return intersection(contents_[i]->parent(), contents_[i]);
		}
	}

	return intersection(false);
}

const bool defaultAccelerator::intersectB(ray& r) const{
	// Just check every shape we have.
	vector<point3> hits;

	point3 ro(r.origin);
	for(unsigned int i=0; i<contents_.size(); i++){
		if(contents_[i]->intersect(r)){
			hits.push_back(point3(r.origin));

			// Reset the ray's origin.
			r.origin = ro;
		}
	}

	if(hits.size() == 0){
		return false;
	}

	// Grab the closest hit.
	float minDist = 10000000.f;
	point3 closestPoint;
	for(unsigned int i=0; i<hits.size(); i++){
		const float dist = (r.origin - hits[i]).length2();
		if(dist < minDist){
			minDist = dist;
			closestPoint = hits[i];
		}
	}

	r.origin = closestPoint;
	return true;
}

void defaultAccelerator::build(const vector<primitive*> contents){
	contents_ = contents;
}
