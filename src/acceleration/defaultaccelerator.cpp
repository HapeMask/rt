#include <vector>
#include "defaultaccelerator.hpp"
#include "geometry/intersectable.hpp"

using namespace std;

const intersection intersect(ray& r) const{
	// Just check every shape we have.
	vector<point3> hits;

	point3 ro(r.origin);
	for(unsigned int i=0; i<shapes_.size(); i++){
		if(shapes_[i]->intersect(r)){
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

void defaultAccelerator::build(const vector<intersectable*> shapes){
	shapes_ = shapes;
}
