#ifndef __RT_BVH__
#define __RT_BVH__

#include "accelerator.hpp"
#include "intersection.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/ray.hpp"

#include <vector>
using namespace std;

enum {LEFT=0, RIGHT=1};
const unsigned short BVH_MAX_PRIMS_PER_LEAF= 4;
enum AXIS{AXIS_X=0,AXIS_Y=1,AXIS_Z=2,AXIS_LEAF=3};

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
        unsigned int rightChild;
        unsigned int prims[2];
    };

    int index;

    AXIS axis;
} bvhNode;

class bvh : public accelerator {
    public:
        bvh() {}

        ~bvh() {
            if(primitiveNodes != NULL){
                delete[] primitiveNodes;
            }

            if(emitterNodes != NULL){
                delete[] emitterNodes;
            }
        }

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;
		virtual const intersection intersectE(ray& r) const;
		virtual const bool intersectEB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        //bvhNode* _build(const aabb& box, unsigned int start, unsigned int end, vector<primitivePtr>& prims, AXIS axis = AXIS_X);
        void _build(const aabb& box,
                unsigned int start, unsigned int end,
                vector<primitivePtr>& prims,
                bvhNode* nodes,
                AXIS axis, int& index);

        const intersection _intersect(const int& index, const ray& r, const vector<primitivePtr>& prims, const bvhNode* nodes) const;
        const bool _intersectB(const int& index, const ray& r, const vector<primitivePtr>& prims, const bvhNode* nodes) const;

        bvhNode* primitiveNodes;
        bvhNode* emitterNodes;

        int numPrimitives, numEmitters;

        vector<primitivePtr> primitives;
        vector<primitivePtr> emitters;
};

inline bool aabbCmpX(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().x() < b->getBounds().mid().x()); }
inline bool aabbCmpY(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().y() < b->getBounds().mid().y()); }
inline bool aabbCmpZ(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().z() < b->getBounds().mid().z()); }
#endif
