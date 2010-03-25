#include <vector>
#include "defaultaccelerator.hpp"
#include "geometry/intersectable.hpp"

using namespace std;

bool defaultAccelerator::intersect(ray& r, point3& p){
	// Just check every shape we have.
	vector<point3> hits;

	for(unsigned int i=0; i<shapes_.size(); i++){
		if(shapes_[i]->intersect(r, p)){
			hits.push_back(point3(p));
		}
	}

	if(hits.size() == 0){
		return false;
	}

	// Grab the closest hit.
	float minDist = 100000.f;
	point3 closestPoint;
	for(unsigned int i=0; i<hits.size(); i++){
		const float dist = (r.origin - hits[i]).length2();
		if(dist < minDist){
			minDist = dist;
			closestPoint = hits[i];
		}
	}

	p = closestPoint;
	return true;
}

void defaultAccelerator::build(const vector<intersectable*> shapes){
	shapes_ = shapes;
}
