#pragma once

#include "accelerator.hpp"
#include "intersection.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/ray.hpp"
#include "datastructs/arraylist.hpp"

enum {LEFT=0, RIGHT=1};
const short BVH_MAX_PRIMS_PER_LEAF = 16;

inline uint8_t nextAxis(uint8_t axis){
    switch(axis){
        case AXIS_X:
          return AXIS_Y;
        case AXIS_Y:
          return AXIS_Z;
        case AXIS_Z:
          return AXIS_X;
        case AXIS_LEAF:
          return AXIS_LEAF;
        default:
          return AXIS_LEAF;
    }
}

typedef struct bn {
    aabb box;
    union{
        int prims[2];
        int rightChild;
    };

    uint8_t axis;
    //mutable uint64_t timesHit;
} bvhNode;

class bvh : public accelerator {
    public:
        bvh() {}

        ~bvh() {
            if(nodes != NULL){
                delete[] nodes;
            }
        }

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        void _build(const aabb& box,
                int start, int end,
                uint8_t axis, int& index);

        const intersection _intersect(const int& index, const ray& r) const;
        const bool _intersectB(const int& index, const ray& r) const;
        const intersection leafTest(const bvhNode& node, const ray& r) const;

        int numNodes;
        int numPrims;
        bvhNode* nodes;
        arraylist<primitive*> primitives;
};
