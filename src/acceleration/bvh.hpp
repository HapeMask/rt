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
/*
const unsigned int AXIS_X = 0;
const unsigned int AXIS_Y = 1;
const unsigned int AXIS_Z = 2;
const unsigned int AXIS_LEAF = 3;
*/
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
        bn* child[2];
        unsigned int prims[2];
    };

    AXIS axis;

    bn() {
        child[LEFT] = NULL;
        child[RIGHT] = NULL;
    }

    ~bn() {
        if(axis != AXIS_LEAF){
            delete child[LEFT];
            delete child[RIGHT];
        }
    }
} bvhNode;

class bvh : public accelerator {
    public:
        bvh();
        ~bvh() {
            if(primitiveRoot != NULL){
                delete primitiveRoot;
            }

            if(emitterRoot != NULL){
                delete emitterRoot;
            }
        }

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;
		virtual const intersection intersectE(ray& r) const;
		virtual const bool intersectEB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        bvhNode* _build(const aabb& box, unsigned int start, unsigned int end, vector<primitivePtr>& prims, AXIS axis = AXIS_X);
        const intersection _intersect(const bvhNode* node, const ray& r, const vector<primitivePtr>& prims) const;
        const bool _intersectB(const bvhNode* node, const ray& r, const vector<primitivePtr>& prims) const;

        bvhNode* primitiveRoot;
        bvhNode* emitterRoot;

        vector<primitivePtr> primitives;
        vector<primitivePtr> emitters;
};

inline bool aabbCmpX(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().x() < b->getBounds().mid().x()); }
inline bool aabbCmpY(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().y() < b->getBounds().mid().y()); }
inline bool aabbCmpZ(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().z() < b->getBounds().mid().z()); }
#endif
