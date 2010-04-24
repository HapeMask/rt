#ifndef __RT_BVH__
#define __RT_BVH__

#include "accelerator.hpp"
#include "intersection.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/ray.hpp"

#include <vector>
using namespace std;

enum AXIS {X=0, Y=1, Z=2, CHILD=3};
enum CHILD {LEFT=0, RIGHT=1};

typedef struct bn {
    aabb box;
    union{
        bn* children[2];
        size_t primRange[2];
    };
    AXIS axis;

    ~bn() {
        if(children[LEFT] != NULL)
            delete children[LEFT];

        if(children[RIGHT] != NULL)
            delete children[RIGHT];
    }
} bvhNode;

class bvh : public accelerator {
    public:
        bvh();
        ~bvh() { delete root; }

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;
		virtual const intersection intersectE(ray& r) const;
		virtual const bool intersectEB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        bvhNode* _build(const aabb& box, unsigned int n, int depth = 0);

        bvhNode* root;
        vector<vector<primitivePtr> > sortedPrims;
        vector<vector<primitivePtr> > sortedEmitters;
};

bool aabbCmpX(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().x() < b->getBounds().mid().x()); }
bool aabbCmpY(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().y() < b->getBounds().mid().y()); }
bool aabbCmpZ(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().z() < b->getBounds().mid().z()); }

#endif
