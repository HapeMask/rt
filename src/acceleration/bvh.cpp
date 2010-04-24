#include "bvh.hpp"
#include "intersection.hpp"
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
    intersection isect = _intersect(primitiveRoot, r, primitives);
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
}

const intersection bvh::_intersect(const bvhNode* node, const ray& r, const vector<primitive*>& prims, const int depth) const{
    float tLeft=0, tRight=0;

    if(!node->box.intersect(r, tLeft)){
        return intersection(false);
    }else if(node->axis == AXIS_LEAF){
        const unsigned int numPrims = node->prims[1] - node->prims[0];

        ray rays[numPrims];
        bool didHit[numPrims];

        for(unsigned int i=0; i<numPrims; i++){
            rays[i] = ray(r);
            didHit[i] = false;
        }

        // Find the intersection points for each primitive in the leaf.
        for(unsigned int i=0; i<numPrims; i++){
            didHit[i] = prims[node->prims[0]+i]->intersect(rays[i]);
        }

        // Check each hit and find the closest.
        int closestIndex = -1;
        float minDist = POS_INF;
        for(unsigned int i=0; i<numPrims; i++){
            if(didHit[i]){
                const float dist = (rays[i].origin - r.origin).length2();
                if(dist < minDist){
                    closestIndex = i;
                    minDist = dist;
                }
            }
        }

        // If we missed all the primitives in the leaf,
        // return no hit, otherwise return the hitpoint.
        if(closestIndex < 0){
            return intersection(false);
        }else{
            primitive* prim = prims[node->prims[0]+closestIndex];
            return intersection(prim->getParent(), prim, (rays[closestIndex].origin - r.origin).length());
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
        const intersection isectLeft = _intersect(node->child[LEFT], r, prims, depth+1);

        if(didIntersectRight){
            const intersection isectRight = _intersect(node->child[RIGHT], r, prims, depth+1);

            if(isectLeft.t < isectRight.t){
                return isectLeft;
            }else{
                return isectRight;
            }
        }else{
            return isectLeft;
        }
    }else if(didIntersectRight){
        return _intersect(node->child[RIGHT], r, prims, depth+1);
    }else{
        return intersection(false);
    }
}

const bool bvh::_intersectB(const bvhNode* node, const ray& r, const vector<primitive*>& prims, const int depth) const{
    float tLeft=0, tRight=0;

    if(!node->box.intersect(r, tLeft)){
        return false;
    }else if(node->axis == AXIS_LEAF){
        const unsigned int numPrims = node->prims[1] - node->prims[0];

        bool didHit[numPrims];

        for(unsigned int i=0; i<numPrims; i++){
            didHit[i] = false;
        }

        // Find the intersection points for each primitive in the leaf.
        for(unsigned int i=0; i<numPrims; i++){
            ray rCopy(r);
            didHit[i] = prims[node->prims[0]+i]->intersect(rCopy);
        }

        // Check to see if we hit anything.
        for(unsigned int i=0; i<numPrims; i++){
            if(didHit[i]){
                return true;
            }
        }
        return false;
    }

    const bool didIntersectLeft = node->child[LEFT]->box.intersect(r, tLeft);
    const bool didIntersectRight = node->child[RIGHT]->box.intersect(r, tRight);

    /*
     * Lots of nested if's below :(
     * TODO: FIX THIS?
     */
    // Check the child boxes to see if we hit them.
    if(didIntersectLeft){
        const bool isectLeft = _intersectB(node->child[LEFT], r, prims, depth+1);

        if(didIntersectRight){
            const bool isectRight = _intersectB(node->child[RIGHT], r, prims, depth+1);

            return (isectLeft || isectRight);
        }else{
            return isectLeft;
        }
    }else if(didIntersectRight){
        return _intersectB(node->child[RIGHT], r, prims, depth+1);
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
            primitives.push_back(p[j].get());
        }
    }

    for(size_t i=0; i<emitterShapes.size(); i++){
        const vector<primitivePtr>& e = emitterShapes[i]->getPrimitives();
        for(size_t j=0; j<e.size(); j++){
            emitters.push_back(e[j].get());
        }
    }

    primitiveRoot = _build(s.getBounds(), 0, primitives.size(), primitives);
    emitterRoot = _build(s.getBounds(), 0, emitters.size(), emitters);
}

bvhNode* bvh::_build(const aabb& box, unsigned int start, unsigned int end, vector<primitive*>& prims, int depth){
    if(start == end){
        return NULL;
    }

    const unsigned short axis = depth % 3;

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
    }
    // Avoid potential integer overflow that:
    //  mid = (start+end)/2
    // would cause.
    const unsigned int mid = start + (end-start)/2;

    aabb leftBox(box);
    aabb rightBox(box);

    leftBox.setMax(axis, prims[mid]->getBounds().max()(axis));
    rightBox.setMin(axis, prims[mid]->getBounds().min()(axis));

    node->child[LEFT] = _build(leftBox, start, mid, prims, depth+1);
    node->child[RIGHT] = _build(rightBox, mid, end, prims, depth+1);
    node->axis = axis;
    return node;
}
