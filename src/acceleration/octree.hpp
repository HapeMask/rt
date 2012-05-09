#pragma once

#include "intersection.hpp" 
#include "accelerator.hpp"

#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

const int OCTREE_MAX_PRIMS_PER_LEAF = 16;
const int OCTREE_MAX_DEPTH = 64;

typedef struct on{
    bool isLeaf;
    aabb box;

    on* children[8];
    vector<primitive*> contents;

    on() : isLeaf(false) {
        for(int i=0;i<8;++i) children[i] = NULL;
    }

    ~on(){
        for(int i=0;i<8;++i) if(children[i]) delete children[i];
    }

} octreeNode;

class octree : public accelerator {
    public:
        octree() {}
        virtual ~octree() {
            delete root;
        }

		virtual const intersection intersect(ray& r) const;
		virtual bool intersectB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        octreeNode* _build(const int depth, const aabb& box,
                const vector<primitive*>& prims);

        const intersection _intersect(const octreeNode* node, ray& r) const;
        bool _intersectB(const octreeNode* node, const ray& r) const;
        const intersection leafTest(const octreeNode& node, const ray& r) const;

        octreeNode* root;
};
