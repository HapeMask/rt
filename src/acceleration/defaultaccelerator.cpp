#include <vector>

#include "defaultaccelerator.hpp"
#include "geometry/primitive.hpp"

using std::shared_ptr;
using std::vector;

intersection defaultAccelerator::intersect(ray& r) const{
    // Grab the closest hit.
    float minDist = POS_INF;
    intersection closestIntersection;

    for(const auto& prim : primitives){
        ray rCopy(r);
        const intersection isect = prim->intersect(rCopy);

        if(isect.t < minDist){
            minDist = isect.t;
            closestIntersection = isect;
        }
    }

    if(minDist == POS_INF) {
        return noIntersect;
    } else {
        r.origin += closestIntersection.t * r.direction;
        return closestIntersection;
    }
}

bool defaultAccelerator::intersectB(const ray& r) const{
    for(const auto& prim : primitives){
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
