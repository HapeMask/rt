#ifndef __RT_BVH__
#define __RT_BVH__

#include "accelerator.hpp"
#include "intersection.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/ray.hpp"

#include <vector>
using namespace std;

enum {LEFT=0, RIGHT=1};
const unsigned short BVH_MAX_PRIMS_PER_LEAF = 4;

inline AXIS nextAxis(AXIS axis){
    switch(axis){
        case AXIS_X:
          return AXIS_Y;
        case AXIS_Y:
          return AXIS_Z;
        case AXIS_Z:
          return AXIS_X;
        case AXIS_LEAF:
          return AXIS_LEAF;
    }
}

typedef struct bn {
    aabb box;
    union{
        unsigned int children[2];
        unsigned int prims[2];
    };

    AXIS axis;
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
        //bvhNode* _build(const aabb& box, unsigned int start, unsigned int end, vector<primitivePtr>& prims, AXIS axis = AXIS_X);
        void _build(const aabb& box,
                unsigned int start, unsigned int end,
                vector<primitivePtr>& prims,
                bvhNode* nodes,
                AXIS axis, int& index);

        const intersection _intersect(const int& index, const ray& r) const;
        const bool _intersectB(const int& index, const ray& r) const;
        const intersection leafTest(const bvhNode& node, const ray& r) const;

        unsigned int numNodes;
        bvhNode* nodes;
        vector<primitivePtr> primitives;
};
#endif
