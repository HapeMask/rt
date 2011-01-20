#include <sys/time.h>
#include <algorithm>
#include <vector>
#include <iostream>

using std::cerr;
using std::endl;

using std::sort;

using std::vector;

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

#ifdef RT_DEBUG
int boxesTested = 0;
#endif

const intersection bvh::intersect(ray& r) const{
    intersection isect = _intersect(0, r);
    if(isect.hit){
        r.origin += isect.t * r.direction;
    }

#ifdef RT_DEBUG
    isect.debugInfo = boxesTested;
    boxesTested = 0;
#endif
    return isect;
}

bool bvh::intersectB(const ray& r) const{
    return _intersectB(0, r);
}

const intersection bvh::leafTest(const bvhNode& node, const ray& r) const{
    const int numPrims = node.prims[1] - node.prims[0];

    // Check each hit and find the closest.
    intersection closestIsect = noIntersect;

    for(int i=0; i<numPrims; ++i){
        ray rCopy(r);
        const intersection isect = primitives[node.prims[0]+i]->intersect(rCopy);
        if(isect.hit){
            if(isect.t < closestIsect.t){
                closestIsect = isect;
            }
        }
    }

    return closestIsect;
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

    const bool didIntersectLeft = nodes[index+1].box.intersect(r, tLeftMin, tLeftMax);
    const bool didIntersectRight = nodes[node.rightChild].box.intersect(r, tRightMin, tRightMax);

#ifdef RT_DEBUG
    boxesTested += 2;
#endif

    // Check the child boxes to see if we hit them.
    if(didIntersectLeft && didIntersectRight){
        const intersection isectLeft = _intersect(index+1, r);

        // Early exit if the intersection is closer than the other box.
        if(isectLeft.hit && isectLeft.t <= tRightMin){
            return isectLeft;
        }else{
            const intersection isectRight = _intersect(node.rightChild, r);

            if(isectLeft.hit && isectLeft.t <= isectRight.t){
                return isectLeft;
            }else{
                return isectRight;
            }
        }
    }else if(didIntersectLeft){
        return _intersect(index+1, r);
    }else if(didIntersectRight){
        return _intersect(node.rightChild, r);
    }else{
        return noIntersect;
    }
}

/**
 * Used for shadow rays, returns as soon as it finds a hit.
 */
bool bvh::_intersectB(const int& index, const ray& r) const{
    float tLeftMin=0, tRightMin=0;
    float tLeftMax=0, tRightMax=0;

    const bvhNode& node = nodes[index];
    if(!node.box.intersect(r, tLeftMin, tLeftMax)){
        return false;
    }else if(node.axis == AXIS_LEAF){
        const int numPrims = node.prims[1] - node.prims[0];

        // Find the intersection points for each primitive in the leaf.
        for(int i=0; i<numPrims; ++i){
            if(primitives[node.prims[0]+i]->intersectB(r)){
                return true;
            }
        }

        return false;
    }

    const bool didIntersectLeft = nodes[index+1].box.intersect(r, tLeftMin, tLeftMax);
    const bool didIntersectRight = nodes[node.rightChild].box.intersect(r, tRightMin, tRightMax);

    // Check the child boxes to see if we hit them.
    if(didIntersectLeft && didIntersectRight){
        return
            (_intersectB(index+1, r) ||
             _intersectB(node.rightChild, r));
    }else if(didIntersectLeft){
        return _intersectB(index+1, r);
    }else if(didIntersectRight){
        return _intersectB(node.rightChild, r);
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
    primitives = vector<primitive*>(numPrims);

    // Fill the list of primitives.
    int k = 0;
    for(size_t i=0; i<shapes.size(); ++i){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); ++j){
            primitives[k] = p[j].get();
            ++k;
        }
    }

    // Allocate space for the trees.
    // A binary tree with N leaves has 2N-1 total nodes.
    //numNodes = 2 * ceil((float)numPrims / (float)BVH_MAX_PRIMS_PER_LEAF) - 1;
    numNodes = 2 * numPrims - 1;

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
        int start, int end,
        uint8_t axis, int& index){

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
            sort(primitives.begin() + start, primitives.begin() + end, aabbMidCmpX);
            break;
        case AXIS_Y:
            sort(primitives.begin() + start, primitives.begin() + end, aabbMidCmpY);
            break;
        case AXIS_Z:
            sort(primitives.begin() + start, primitives.begin() + end, aabbMidCmpZ);
            break;
        case AXIS_LEAF:
            cerr << "Hit leaf twice in BVH build recursion. This should never happen." << endl;
            return;
    }

    // Rounding ensures that the leaves are well-filled.
    // (# leaves with nPrims < BVH_MAX_PRIMS_PER_LEAF is 0 or 1)
    //const int mid = roundUpToMultiple((start+end)/2, BVH_MAX_PRIMS_PER_LEAF);
    const int mid = (start+end)/2;

    aabb leftBox(primitives[start]->getBounds());
    aabb rightBox(primitives[mid]->getBounds());

    for(int i=start; i<mid; ++i){
        leftBox = mergeAabb(leftBox, primitives[i]->getBounds());
    }

    for(int i=mid; i<end; ++i){
        rightBox = mergeAabb(rightBox, primitives[i]->getBounds());
    }

    // Put the node into the storage array.
    node.axis = axis;
    nodes[index] = node;

    const int savedIndex(index);

    // Set the next free position.
    ++index;

    // The left side will be built in DFS order, after which index will point to the next
    // free node.
    _build(leftBox, start, mid, nextAxis(axis), index);

    // Set the next free position.
    ++index;
    nodes[savedIndex].rightChild = index;

    _build(rightBox, mid, end, nextAxis(axis), index);
}
