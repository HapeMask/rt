#include "bvh.hpp"
#include "intersection.hpp"
#include "utility.hpp"

#include "geometry/triangle.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

#include "scene/scene.hpp"

#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/sse.hpp"

#include <sys/time.h>
#include <algorithm>

#include "datastructs/arraylist.hpp"
using namespace std;

//DBG
int boxesTested = 0;

const intersection bvh::intersect(ray& r) const{
    intersection isect = _intersect(0, r);
    if(isect.hit){
        r.origin += isect.t * r.direction;
    }

    //DBG
    isect.debugInfo = boxesTested;
    boxesTested = 0;
    return isect;
}

const bool bvh::intersectB(const ray& r) const{
    return _intersectB(0, r);
}

const intersection bvh::leafTest(const bvhNode& node, const ray& r) const{
    const unsigned int numPrims = node.prims[1] - node.prims[0];

    // Check each hit and find the closest.
    primitive* closestPrim;
    ray closestRay;
    intersection closestIsect;
    bool didHit = false;
    float minDist = POS_INF;

    for(unsigned int i=0; i<numPrims; ++i){
        ray rCopy(r);
        const intersection isect = primitives[node.prims[0]+i]->intersect(rCopy);
        if(isect.hit){
            didHit = true;

            if(isect.t < minDist){
                closestPrim = primitives[node.prims[0]+i];
                minDist = isect.t;
                closestRay = rCopy;
                closestIsect = isect;
            }
        }
    }

    // If we missed all the primitives in the leaf,
    // return no hit, otherwise return the hitpoint.
    if(didHit){
        return closestIsect;
    }else{
        return noIntersect;
    }
}

const intersection bvh::_intersect(const int& index, const ray& r) const{
    float tLeftMin=0, tRightMin=0;
    float tLeftMax=0, tRightMax=0;

    const bvhNode& node = nodes[index];

    if(!node.box.intersect(r, tLeftMin, tLeftMax)){
        return noIntersect;
    }else if(node.axis == AXIS_LEAF){
        return leafTest(node, r);
    }

    const bool didIntersectLeft = nodes[node.children[LEFT]].box.intersect(r, tLeftMin, tLeftMax);
    const bool didIntersectRight = nodes[node.children[RIGHT]].box.intersect(r, tRightMin, tRightMax);

    //DBG
    boxesTested += 2;

    // Check the child boxes to see if we hit them.
    if(didIntersectLeft && didIntersectRight){
        const intersection isectLeft = _intersect(node.children[LEFT], r);
        const intersection isectRight = _intersect(node.children[RIGHT], r);

        // Early exit if the intersection is closer than the other box.
        if(isectLeft.t <= tRightMin && isectLeft.hit){
            return isectLeft;
        }else{
            const intersection isectRight = _intersect(node.children[RIGHT], r);
            if(isectLeft.t <= isectRight.t && isectLeft.hit){
                return isectLeft;
            }else{
                return isectRight;
            }
        }
    }else if(didIntersectLeft){
        return _intersect(node.children[LEFT], r);
    }else if(didIntersectRight){
        return _intersect(node.children[RIGHT], r);
    }else{
        return noIntersect;
    }
}

/**
 * Used for shadow rays, returns as soon as it finds a hit.
 */
const bool bvh::_intersectB(const int& index, const ray& r) const{
    float tLeftMin=0, tRightMin=0;
    float tLeftMax=0, tRightMax=0;

    const bvhNode& node = nodes[index];
    if(!node.box.intersect(r, tLeftMin, tLeftMax)){
        return false;
    }else if(node.axis == AXIS_LEAF){
        const unsigned int numPrims = node.prims[1] - node.prims[0];

        // Find the intersection points for each primitive in the leaf.
        for(unsigned int i=0; i<numPrims; ++i){
            ray rCopy(r);
            if(primitives[node.prims[0]+i]->intersect(rCopy).hit){
                return true;
            }
        }
        return false;
    }

    const bool didIntersectLeft = nodes[node.children[LEFT]].box.intersect(r, tLeftMin, tLeftMax);
    const bool didIntersectRight = nodes[node.children[RIGHT]].box.intersect(r, tRightMin, tRightMax);

    // Check the child boxes to see if we hit them.
    if(didIntersectLeft && didIntersectRight){
        return
            (_intersectB(node.children[LEFT], r) ||
             _intersectB(node.children[RIGHT], r));
    }else if(didIntersectLeft){
        return _intersectB(node.children[LEFT], r);
    }else if(didIntersectRight){
        return _intersectB(node.children[RIGHT], r);
    }else{
        return false;
    }
}

void bvh::build(const scene& s){
    const vector<shapePtr>& shapes = s.getShapes();

    numPrims = 0;
    for(size_t i=0; i<shapes.size(); ++i){
        numPrims += shapes[i]->getPrimitives().size();
    }

    // Allocate primitive array.
    //primitives = new primitive*[numPrims];
    primitives = arraylist<primitive*>(numPrims);

    // Fill the list of primitives.
    unsigned int k = 0;
    for(size_t i=0; i<shapes.size(); ++i){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); ++j){
            primitives[k] = p[j].get();
            ++k;
        }
    }

    // Allocate space for the trees.
    // A binary tree with N leaves has 2N-1 total nodes.
    numNodes = 2 * ceil((float)numPrims / (float)BVH_MAX_PRIMS_PER_LEAF) - 1;
    //numNodes = 2 * numPrims - 1;

    nodes = new bvhNode[numNodes];

    int index = 0;
    cerr << "Building BVH..." << endl;
	struct timeval start, end;
	gettimeofday(&start, NULL);

    _build(s.getBounds(), 0, numPrims, AXIS_X, index);

	gettimeofday(&end, NULL);
	float sec = end.tv_sec - start.tv_sec;
	sec += (end.tv_usec - start.tv_usec) / 1e6;
	cerr << "Built BVH in " << sec << "s." << endl;
}

void bvh::_build(const aabb& box,
        unsigned int start, unsigned int end,
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
            sort(primitives.begin() + start, primitives.begin() + end, aabbCmpX);
            break;
        case AXIS_Y:
            sort(primitives.begin() + start, primitives.begin() + end, aabbCmpY);
            break;
        case AXIS_Z:
            sort(primitives.begin() + start, primitives.begin() + end, aabbCmpZ);
            break;
        case AXIS_LEAF:
            cerr << "Hit leaf twice in BVH build recursion. This should never happen." << endl;
            return;
    }

    // Rounding ensures that the leaves are well-filled.
    // (# leaves with nPrims < BVH_MAX_PRIMS_PER_LEAF is 0 or 1)
    //const unsigned int mid = roundUpToMultiple((start+end)/2, BVH_MAX_PRIMS_PER_LEAF);
    const unsigned int mid = (start+end)/2;

    aabb leftBox(primitives[start]->getBounds());
    aabb rightBox(primitives[mid]->getBounds());

    for(unsigned int i=start; i<mid; ++i){
        leftBox = mergeAabb(leftBox, primitives[i]->getBounds());
    }

    for(unsigned int i=mid; i<end; ++i){
        rightBox = mergeAabb(rightBox, primitives[i]->getBounds());
    }

    // Put the node into the storage array.
    node.axis = axis;
    node.children[LEFT] = index+1;
    nodes[index] = node;

    const int savedIndex(index);

    // Set the next free position.
    ++index;

    // The left side will be built in DFS order, after which index will point to the next
    // free node.
    _build(leftBox, start, mid, nextAxis(axis), index);

    // Set the next free position.
    ++index;
    nodes[savedIndex].children[1] = index;

    _build(rightBox, mid, end, nextAxis(axis), index);
}
