#include "bvh.hpp"
#include "intersection.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"
#include "mathlib/ray.hpp"
#include "scene/scene.hpp"

#include <vector>
#include <algorithm>
using namespace std;

bvh::bvh() : sortedPrims(3), sortedEmitters(3)
{}

const intersection bvh::intersect(ray& r) const{
}

const bool bvh::intersectB(const ray& r) const{
}

const intersection bvh::intersectE(ray& r) const{
}

const bool bvh::intersectEB(const ray& r) const{
}


void bvh::build(const scene& s){
    const vector<shapePtr>& shapes = s.getShapes();
    const vector<shapePtr>& emitters = s.getShapes();

    // Fill the list of primitives and emitter primitives.
    vector<primitivePtr> prims;
    for(size_t i=0; i<shapes.size(); i++){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); j++){
            prims.push_back(p[j]);
        }
    }

    vector<primitivePtr> eprims;
    for(size_t i=0; i<emitters.size(); i++){
        const vector<primitivePtr>& p = emitters[i]->getPrimitives();
        for(size_t j=0; j<p.size(); j++){
            eprims.push_back(p[j]);
        }
    }

    // Construct and sort lists.
    vector<primitivePtr> xPrims(prims);
    vector<primitivePtr> yPrims(prims);
    vector<primitivePtr> zPrims(prims);

    vector<primitivePtr> xEmitters(eprims);
    vector<primitivePtr> yEmitters(eprims);
    vector<primitivePtr> zEmitters(eprims);

    sort(xPrims.begin(), xPrims.end(), aabbCmpX);
    sort(xPrims.begin(), yPrims.end(), aabbCmpY);
    sort(xPrims.begin(), zPrims.end(), aabbCmpZ);
    sort(xEmitters.begin(), xEmitters.end(), aabbCmpX);
    sort(xEmitters.begin(), yEmitters.end(), aabbCmpY);
    sort(xEmitters.begin(), zEmitters.end(), aabbCmpZ);

    sortedPrims[X] = xPrims;
    sortedPrims[Y] = yPrims;
    sortedPrims[Z] = zPrims;

    sortedEmitters[X] = xEmitters;
    sortedEmitters[Y] = yEmitters;
    sortedEmitters[Z] = zEmitters;

    _build(s.getBounds());
}

bvhNode* bvh::_build(const aabb& box, int depth){
}
