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
const unsigned int AXIS_X = 0;
const unsigned int AXIS_Y = 1;
const unsigned int AXIS_Z = 2;
const unsigned int AXIS_LEAF = 3;

typedef struct bn {
    aabb box;
    union{
        bn* child[2];
        unsigned int prims[2];
    };

    unsigned short axis;

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

        bvhNode* primitiveRoot;
        bvhNode* emitterRoot;

    private:
        bvhNode* _build(const aabb& box, unsigned int start, unsigned int end, vector<primitive*>& prims, int depth = 0);
        const intersection _intersect(const bvhNode* node, const ray& r, const vector<primitive*>& prims, const int depth = 0) const;
        const bool _intersectB(const bvhNode* node, const ray& r, const vector<primitive*>& prims, const int depth = 0) const;

        vector<primitive*> primitives;
        vector<primitive*> emitters;
};

inline bool aabbCmpX(primitive* a, primitive* b) { return (a->getBounds().mid().x() < b->getBounds().mid().x()); }
inline bool aabbCmpY(primitive* a, primitive* b) { return (a->getBounds().mid().y() < b->getBounds().mid().y()); }
inline bool aabbCmpZ(primitive* a, primitive* b) { return (a->getBounds().mid().z() < b->getBounds().mid().z()); }
#endif
