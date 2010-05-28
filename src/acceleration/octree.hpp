#pragma once

#include "intersection.hpp" 
#include "accelerator.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

const unsigned int OCTREE_MAX_PRIMS_PER_LEAF = 4;
typedef struct on{
    aabb box;
    on* leftChild;
    on* rightChild;

    int nPrims;
    unsigned int primIndices[OCTREE_MAX_PRIMS_PER_LEAF];

    ~on(){
        if(leftChild) delete leftChild;
        if(rightChild) delete rightChild;
    }

} octreeNode;

class octree : public accelerator {
    public:
        octree() {}

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        void _build(octreeNode* node);

        octreeNode* root;
        vector<primitivePtr> prims;
};
