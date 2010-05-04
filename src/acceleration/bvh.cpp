#include "bvh.hpp"
#include "intersection.hpp"

#include "geometry/triangle.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

#include "utility.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"
#include "scene/scene.hpp"

#include <vector>
#include <algorithm>
using namespace std;

const intersection bvh::intersect(ray& r) const{
    const intersection isect = _intersect(0, r, primitives, primitiveNodes);
    if(isect.hit){
        r.origin += isect.t * r.direction;
    }

    return isect;
}

const bool bvh::intersectB(const ray& r) const{
    return _intersectB(0, r, primitives, primitiveNodes);
}

const intersection bvh::intersectE(ray& r) const{
    intersection isect = _intersect(0, r, emitters, emitterNodes);
    if(isect.hit){
        r.origin += isect.t * r.direction;
    }

    return isect;
}

const bool bvh::intersectEB(const ray& r) const{
    return _intersectB(0, r, emitters, emitterNodes);
}

const intersection bvh::_intersect(const int& index, const ray& r, const vector<primitivePtr>& prims, const bvhNode* nodes) const{
    float tLeft=0, tRight=0;

    const bvhNode& node = nodes[index];

    if(!node.box.intersect(r, tLeft)){
        return intersection(false);
    }else if(node.axis == AXIS_LEAF){
        const unsigned int numPrims = node.prims[1] - node.prims[0];

        // Check each hit and find the closest.
        primitivePtr closestPrim;
        ray closestRay;
        bool didHit = false;
        float minDist = POS_INF;
        for(unsigned int i=0; i<numPrims; ++i){
            ray rCopy(r);
            if(prims[node.prims[0]+i]->intersect(rCopy)){
                const float dist = (rCopy.origin - r.origin).length2();
                didHit = true;

                if(dist < minDist){
                    closestPrim = prims[node.prims[0]+i];
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

    const bvhNode& leftChild = nodes[index+1];
    const bvhNode& rightChild = nodes[node.rightChild];
    const bool didIntersectLeft = leftChild.box.intersect(r, tLeft);
    const bool didIntersectRight = rightChild.box.intersect(r, tRight);

    /*
     * Lots of nested if's below :(
     * TODO: FIX THIS?
     */
    // Check the child boxes to see if we hit them.
    if(didIntersectLeft){
        const intersection isectLeft = _intersect(index+1, r, prims, nodes);

        if(didIntersectRight){
            // Ray intersects both boxes.
            const intersection isectRight = _intersect(node.rightChild, r, prims, nodes);

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
        return _intersect(node.rightChild, r, prims, nodes);
    }else{
        // Ray misses both boxes.
        return intersection(false);
    }
}

/**
 * Used for shadow rays, returns as soon as it finds a hit.
 */
const bool bvh::_intersectB(const int& index, const ray& r, const vector<primitivePtr>& prims, const bvhNode* nodes) const{
    float tLeft=0, tRight=0;

    const bvhNode& node = nodes[index];
    if(!node.box.intersect(r, tLeft)){
        return false;
    }else if(node.axis == AXIS_LEAF){
        const unsigned int numPrims = node.prims[1] - node.prims[0];

        // Find the intersection points for each primitive in the leaf.
        for(unsigned int i=0; i<numPrims; ++i){
            ray rCopy(r);
            if(prims[node.prims[0]+i]->intersect(rCopy)){
                return true;
            }
        }
        return false;
    }

    const bvhNode& leftChild = nodes[index+1];
    const bvhNode& rightChild = nodes[node.rightChild];
    const bool didIntersectLeft = leftChild.box.intersect(r, tLeft);
    const bool didIntersectRight = rightChild.box.intersect(r, tRight);

    // Check the child boxes to see if we hit them.
    if(didIntersectLeft){
        const bool isectLeft = _intersectB(index+1, r, prims, nodes);

        if(didIntersectRight){
            const bool isectRight = _intersectB(node.rightChild, r, prims, nodes);

            return (isectLeft || isectRight);
        }else{
            return isectLeft;
        }
    }else if(didIntersectRight){
        return _intersectB(node.rightChild, r, prims, nodes);
    }else{
        return false;
    }
}

void bvh::build(const scene& s){
    const vector<shapePtr>& shapes = s.getShapes();
    const vector<shapePtr>& emitterShapes = s.getEmitters();

    // Fill the list of primitives and emitter primitives.
    for(size_t i=0; i<shapes.size(); ++i){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); ++j){
            primitives.push_back(p[j]);
        }
    }

    for(size_t i=0; i<emitterShapes.size(); ++i){
        const vector<primitivePtr>& e = emitterShapes[i]->getPrimitives();
        for(size_t j=0; j<e.size(); ++j){
            primitives.push_back(e[j]);
            emitters.push_back(e[j]);
        }
    }

    const unsigned int numPrims = primitives.size();
    const unsigned int numEmitters = emitters.size();

    // Allocate space for the trees.
    // A binary tree with N leaves has 2N-1 total nodes.
    const int numPrimNodes = 2 * ceil((float)numPrims / (float)BVH_MAX_PRIMS_PER_LEAF) - 1;
    const int numEmitterNodes = 2 * ceil((float)numEmitters / (float)BVH_MAX_PRIMS_PER_LEAF) - 1;

    if(numPrimNodes > 0){
        primitiveNodes = new bvhNode[numPrimNodes];
    }else{
        primitiveNodes = NULL;
    }

    if(numEmitterNodes > 0){
        emitterNodes = new bvhNode[numEmitterNodes];
    }else{
        emitterNodes = NULL;
    }

    int index = 0;
    _build(s.getBounds(), 0, primitives.size(), primitives, primitiveNodes, AXIS_X, index);

    index = 0;
    _build(s.getBounds(), 0, emitters.size(), emitters, emitterNodes, AXIS_X, index);
}

void bvh::_build(const aabb& box,
        unsigned int start, unsigned int end,
        vector<primitivePtr>& prims,
        bvhNode* nodes,
        AXIS axis, int& index){

    if(start == end){
        return;
    }

    bvhNode node;
    node.box = box;
    if((end-start) <= BVH_MAX_PRIMS_PER_LEAF){
        node.prims[0] = start;
        node.prims[1] = end;
        node.axis = AXIS_LEAF;
        nodes[index] = node;
        return;
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
            return;
    }

    // Avoid potential integer overflow that:
    //  mid = (start+end)/2
    // would cause.
    //
    // Rounding ensures that the leaves are well-filled.
    // (# leaves with nPrims < BVH_MAX_PRIMS_PER_LEAF is 0 or 1)
    const unsigned int mid =
        roundUpToMultiple(start + (end-start)/2, BVH_MAX_PRIMS_PER_LEAF);

    aabb leftBox(prims[start]->getBounds());
    aabb rightBox(prims[mid]->getBounds());

    // TODO: Find a more efficient way of doing this.
    // See r288-ish for one way that almost worked but
    // not quite.
    for(unsigned int i=start; i<mid; ++i){
        leftBox = mergeAabb(leftBox, prims[i]->getBounds());
    }

    for(unsigned int i=mid; i<end; ++i){
        rightBox = mergeAabb(rightBox, prims[i]->getBounds());
    }

    // Put the node into the storage array.
    node.axis = axis;
    nodes[index] = node;
    const int savedIndex(index);

    // Set the next free position.
    ++index;

    // The left side will be built in DFS order, after which index will point to the next
    // free node.
    _build(leftBox, start, mid, prims, nodes, nextAxis(axis), index);

    // Set the next free position.
    ++index;
    nodes[savedIndex].rightChild = index;

    _build(rightBox, mid, end, prims, nodes, nextAxis(axis), index);
}
