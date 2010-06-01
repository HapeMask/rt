#pragma once

#include "intersection.hpp" 
#include "accelerator.hpp"

#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

#include "datastructs/linkedlist.hpp"

const unsigned int OCTREE_MAX_PRIMS_PER_LEAF = 4;
typedef struct on{
    uint8_t axis;
    float splitPlane;

    on* leftChild;
    on* rightChild;

    linkedlist<primitivePtr>* contents;

    on() :
        leftChild(NULL),
        rightChild(NULL),
        contents(new linkedlist<primitivePtr>())
    {}

    ~on(){
        if(leftChild) delete leftChild;
        if(rightChild) delete rightChild;
        if(contents) delete contents;
    }

} octreeNode;

class octree : public accelerator {
    public:
        octree() {}
        virtual ~octree() {
            delete root;
        }

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

		virtual void build(const scene& s);

    private:
        void _build(octreeNode* node);

        octreeNode* root;
};
