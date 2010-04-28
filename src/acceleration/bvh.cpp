#include "bvh.hpp"
#include "intersection.hpp"

#include "geometry/triangle.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"
#include "scene/scene.hpp"

#include <vector>
#include <algorithm>
using namespace std;

bvh::bvh() : primitiveRoot(NULL), emitterRoot(NULL)
{}

const intersection bvh::intersect(ray& r) const{
    const intersection isect = _intersect(primitiveRoot, r, primitives);
    if(isect.hit){
        r.origin += isect.t * r.direction;
    }

    return isect;
}

const bool bvh::intersectB(const ray& r) const{
    return _intersectB(primitiveRoot, r, primitives);
}

const intersection bvh::intersectE(ray& r) const{
    intersection isect = _intersect(emitterRoot, r, emitters);
    if(isect.hit){
        r.origin += isect.t * r.direction;
    }

    return isect;
}

const bool bvh::intersectEB(const ray& r) const{
    return _intersectB(emitterRoot, r, emitters);
}

const intersection bvh::_intersect(const bvhNode* node, const ray& r, const vector<primitivePtr>& prims) const{
    float tLeft=0, tRight=0;

    if(!node->box.intersect(r, tLeft)){
        return intersection(false);
    }else if(node->axis == AXIS_LEAF){
        const unsigned int numPrims = node->prims[1] - node->prims[0];

        // Check each hit and find the closest.
        primitivePtr closestPrim;
        ray closestRay;
        bool didHit = false;
        float minDist = POS_INF;
        for(unsigned int i=0; i<numPrims; i++){
            ray rCopy(r);
            if(prims[node->prims[0]+i]->intersect(rCopy)){
                const float dist = (rCopy.origin - r.origin).length2();
                didHit = true;

                if(dist < minDist){
                    closestPrim = prims[node->prims[0]+i];
                    minDist = dist;
                    closestRay = rCopy;
                }
            }
        }

        // If we missed all the primitives in the leaf,
        // return no hit, otherwise return the hitpoint.
        if(didHit){
            return intersection(closestPrim->getParent(), closestPrim, (closestRay.origin - r.origin).length());
        }else{
            return intersection(false);
        }
    }

    const bool didIntersectLeft = node->child[LEFT]->box.intersect(r, tLeft);
    const bool didIntersectRight = node->child[RIGHT]->box.intersect(r, tRight);

    /*
     * Lots of nested if's below :(
     * TODO: FIX THIS?
     */
    // Check the child boxes to see if we hit them.
    if(didIntersectLeft){
        const intersection isectLeft = _intersect(node->child[LEFT], r, prims);

        if(didIntersectRight){
            // Ray intersects both boxes.
            const intersection isectRight = _intersect(node->child[RIGHT], r, prims);

            // Return the closest actual hit.
            if(isectLeft.t < isectRight.t){
                return isectLeft;
            }else{
                return isectRight;
            }
        }else{
            // Ray only intersects left.
            return isectLeft;
        }
    }else if(didIntersectRight){
        // Ray only intersects right.
        return _intersect(node->child[RIGHT], r, prims);
    }else{
        // Ray misses both boxes.
        return intersection(false);
    }
}

/**
 * Used for shadow rays, returns as soon as it finds a hit.
 */
const bool bvh::_intersectB(const bvhNode* node, const ray& r, const vector<primitivePtr>& prims) const{
    float tLeft=0, tRight=0;

    if(!node->box.intersect(r, tLeft)){
        return false;
    }else if(node->axis == AXIS_LEAF){
        const unsigned int numPrims = node->prims[1] - node->prims[0];

        // Find the intersection points for each primitive in the leaf.
        for(unsigned int i=0; i<numPrims; i++){
            ray rCopy(r);
            if(prims[node->prims[0]+i]->intersect(rCopy)){
                return true;
            }
        }
        return false;
    }

    const bool didIntersectLeft = node->child[LEFT]->box.intersect(r, tLeft);
    const bool didIntersectRight = node->child[RIGHT]->box.intersect(r, tRight);

    // Check the child boxes to see if we hit them.
    if(didIntersectLeft){
        const bool isectLeft = _intersectB(node->child[LEFT], r, prims);

        if(didIntersectRight){
            const bool isectRight = _intersectB(node->child[RIGHT], r, prims);

            return (isectLeft || isectRight);
        }else{
            return isectLeft;
        }
    }else if(didIntersectRight){
        return _intersectB(node->child[RIGHT], r, prims);
    }else{
        return false;
    }
}

void bvh::build(const scene& s){
    const vector<shapePtr>& shapes = s.getShapes();
    const vector<shapePtr>& emitterShapes = s.getEmitters();

    // Fill the list of primitives and emitter primitives.
    for(size_t i=0; i<shapes.size(); i++){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); j++){
            primitives.push_back(p[j]);
        }
    }

    for(size_t i=0; i<emitterShapes.size(); i++){
        const vector<primitivePtr>& e = emitterShapes[i]->getPrimitives();
        for(size_t j=0; j<e.size(); j++){
            emitters.push_back(e[j]);
        }
    }

    primitiveRoot = _build(s.getBounds(), 0, primitives.size(), primitives);
    emitterRoot = _build(s.getBounds(), 0, emitters.size(), emitters);
}

bvhNode* bvh::_build(const aabb& box, unsigned int start, unsigned int end, vector<primitivePtr>& prims, AXIS axis){
    if(start == end){
        return NULL;
    }

    bvhNode* node = new bvhNode;
    node->box = box;
    if((end-start) <= BVH_MAX_PRIMS_PER_LEAF){
        node->prims[0] = start;
        node->prims[1] = end;
        node->axis = AXIS_LEAF;
        return node;
    }

    // Sort the current chunk of the list.
    switch(axis){
        case AXIS_X:
            sort(prims.begin()+start, prims.begin()+end, aabbCmpX);
            break;
        case AXIS_Y:
            sort(prims.begin()+start, prims.begin()+end, aabbCmpY);
            break;
        case AXIS_Z:
            sort(prims.begin()+start, prims.begin()+end, aabbCmpZ);
            break;
        case AXIS_LEAF:
            cerr << "Hit leaf twice in BVH build recursion. This should never happen." << endl;
            return NULL;
    }

    // Avoid potential integer overflow that:
    //  mid = (start+end)/2
    // would cause.
    const unsigned int mid = start + (end-start)/2;

    aabb leftBox(prims[start]->getBounds());
    aabb rightBox(prims[mid]->getBounds());

    // TODO: Find a more efficient way of doing this.
    // See r288-ish for one way that almost worked but
    // not quite.
    for(unsigned int i=start; i<mid; i++){
        leftBox = mergeAabb(leftBox, prims[i]->getBounds());
    }

    for(unsigned int i=mid; i<end; i++){
        rightBox = mergeAabb(rightBox, prims[i]->getBounds());
    }

    node->child[LEFT] = _build(leftBox, start, mid, prims, nextAxis(axis));
    node->child[RIGHT] = _build(rightBox, mid, end, prims, nextAxis(axis));
    node->axis = axis;
    return node;
}

int bvh::size(){
    return _size(primitiveRoot);
}

int bvh::_size(bvhNode* node){
    if(node->axis == AXIS_LEAF){
        return 1;
    }

    return _size(node->child[LEFT]) + _size(node->child[RIGHT]) + 1;
}
