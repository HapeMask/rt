#pragma once

#include <vector>
using std::vector;

#include "accelerator.hpp"
#include "intersection.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/ray.hpp"

enum {LEFT=0, RIGHT=1};
constexpr int BVH_MAX_PRIMS_PER_LEAF = 16;
constexpr int BVH_NUM_SPLIT_CANDIDATES = 64;

typedef struct bn {
    aabb box;
    int primitives[2];
    int rightChild;
} bvhNode;

class bvh : public accelerator {
    public:
        bvh() {}

        virtual intersection intersect(ray& r) const;
        virtual bool intersectB(const ray& r) const;

        virtual void build(const scene& s);

    private:
        /**
         *  Recursively builds the portion of the BVH subtree stored in the
         *  array from start to end.
         *
         *  @param box: The bounding box for the root of the current BVH tree.
         *
         *  @param start: The array index of the first primitive in this BVH's
         *  tree.
         *
         *  @param end: The array index of the last primitive in this BVH's
         *  tree.
         *
         *  @param index: the index of the next free position in the node
         *  array.
         *
         *  @return: the next free position in the node array.
         */
        int _build(const aabb& box, int start, int end, int index);

        intersection _intersect(const int& index, const ray& r) const;
        bool _intersectB(const int& index, const ray& r) const;

        intersection leafTest(const bvhNode& node, const ray& r) const;

        int numNodes;
        int numPrims;
        vector<bvhNode> nodes;
        vector<primitive*> primitives;
};
