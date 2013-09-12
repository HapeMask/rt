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
#include "samplers/samplers.hpp"

#include "mathlib/ray.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/sse.hpp"

#ifdef RT_DEBUG
int boxesTested = 0;
#endif

intersection bvh::intersect(ray& r) const{
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

intersection bvh::leafTest(const bvhNode& node, const ray& r) const{
    const int primsToTest = node.primitives[1] - node.primitives[0];

    // Check each hit and find the closest.
    intersection closestIsect = noIntersect;

    for(int i=0; i<primsToTest; ++i){
        ray rCopy(r);
        const intersection isect = primitives[node.primitives[0]+i]->intersect(rCopy);
        if(isect.hit){
            if(isect.t < closestIsect.t){
                closestIsect = isect;
            }
        }
    }

    return closestIsect;
}

intersection bvh::_intersect(const int& index, const ray& r) const{
    float tLeftMin=0, tRightMin=0;
    float tLeftMax=0, tRightMax=0;

    const bvhNode& node = nodes[index];

    if(!node.box.intersect(r, tLeftMin, tLeftMax)){
        return noIntersect;
    }else if(node.rightChild == -1){
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
    }else if(node.rightChild == -1){
        const int primsToTest = node.primitives[1] - node.primitives[0];

        // Find the intersection points for each primitive in the leaf.
        for(int i=0; i<primsToTest; ++i){
            if(primitives[node.primitives[0]+i]->intersectB(r)){
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
    numPrims = 0;
    const vector<shared_ptr<shape>>& shapes = s.getShapes();
    for(const auto& shape : s.getShapes()){
        numPrims += shape->getPrimitives().size();
    }

    primitives = vector<primitive*>(numPrims);

    // Fill the list of primitives.
    int k = 0;
    for(const auto& shape : shapes){
        for(const auto& prim : shape->getPrimitives()){
            primitives[k] = prim.get();
            ++k;
        }
    }

    // Allocate space for the trees.
    numNodes = 2 * numPrims - 1;
    nodes.reserve(numNodes);

    int index = 0;
    cerr << "Building BVH..." << endl;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    _build(s.getBounds(), 0, numPrims, index);

    gettimeofday(&end, NULL);
    float sec = end.tv_sec - start.tv_sec;
    sec += (end.tv_usec - start.tv_usec) / 1e6;
    cerr << "Built BVH in " << sec << "s." << endl;
}

int bvh::_build(const aabb& box, int start, int end, int index){
    bvhNode node;
    node.box = box;

    if((end-start) <= BVH_MAX_PRIMS_PER_LEAF){
        node.primitives[0] = start;
        node.primitives[1] = end;
        node.rightChild = -1;
        nodes[index] = node;
        return index+1;
    }

    int bestSplit = start+1;
    float bestCost = MAX_FLOAT;
    aabb bestLeftBox, bestRightBox;
    vector<primitive*> tempPrimitives(primitives);
    vector<int> splits(BVH_NUM_SPLIT_CANDIDATES);

    for(int axis=0; axis<3; ++axis) {
        auto cmp = (axis == AXIS_X) ? aabbMidCmpX : ((axis == AXIS_Y) ? aabbMidCmpY : aabbMidCmpZ);
        sort(tempPrimitives.begin() + start, tempPrimitives.begin() + end, cmp);

        for(int i=0; i < BVH_NUM_SPLIT_CANDIDATES; ++i) {
            splits[i] = sampleRange(sampleUniform(), start+1, end-2);
        }

        for(const int& split : splits) {
            aabb leftBox(tempPrimitives[start]->getBounds());
            aabb rightBox(tempPrimitives[split]->getBounds());
            float areaLeft=0, areaRight=0;

            for(int i=start; i<split; ++i){
                leftBox = mergeAabb(leftBox, tempPrimitives[i]->getBounds());
                areaLeft += tempPrimitives[i]->area();
            }

            for(int i=split; i<end; ++i){
                rightBox = mergeAabb(rightBox, tempPrimitives[i]->getBounds());
                areaRight += tempPrimitives[i]->area();
            }

            const float costSAH = areaLeft * (split - start) + areaRight * (end - split);
            if(costSAH < bestCost) {
                bestCost = costSAH;
                bestSplit = split;
                bestLeftBox = leftBox;
                bestRightBox = rightBox;
                primitives = tempPrimitives;
            }
        }
    }

    // Put the node into the storage array.
    nodes[index] = node;

    const int leftIndex = _build(bestLeftBox, start, bestSplit, index+1);
    const int rightIndex = _build(bestRightBox, bestSplit, end, leftIndex+1);

    nodes[index].rightChild = leftIndex + 1;
    return rightIndex;
}
