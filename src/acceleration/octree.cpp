#include "octree.hpp"
#include "intersection.hpp" 

#include "scene/scene.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

#include <algorithm>
using namespace std;

void octree::build(const scene& s){
    const vector<shapePtr>& shapes = s.getShapes();
    arraylist<primitive*> prims;

    // Fill the list of primitives.
    for(size_t i=0; i<shapes.size(); ++i){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); ++j){
            prims.add(p[j].get());
        }
    }

    /*
    const aabb& box = s.getBounds();
    root = new octreeNode();

    for(int i=0;i<3;++i){
        root->splitPlanes[i] = box.mid()(i);
    }
    */

    root = _build(0, s.getBounds(), prims);
}

octreeNode* octree::_build(const int depth, const aabb& box,
        const arraylist<primitive*>& prims){

    arraylist<primitive*> boxPrims;

    // Find the primitives that intersect this node's box.
    for(int i=0; i<prims.size(); ++i){
        if(prims[i]->getBounds().intersect(box)){
            boxPrims.add(prims[i]);
        }
    }

    // Empty space.
    if(boxPrims.size() == 0){
        return NULL;
    }

    octreeNode* node = new octreeNode();
    node->box = box;

    // Make a leaf.
    if(boxPrims.size() <= OCTREE_MAX_PRIMS_PER_LEAF || depth >= OCTREE_MAX_DEPTH){
        node->isLeaf = true;
        node->contents = boxPrims;
        return node;
    }

    /*
    for(unsigned int i=0;i<3;++i){
        node->splitPlanes[i] = box.mid()(i);
    }
    */

    // TOOD: Make this less dumb.
    const aabb boxes[8] = {
    };

    for(int i=0;i<8;++i){
        node->children[i] = _build(depth+1, boxes[i], boxPrims);
    }

    return node;
}

const intersection octree::intersect(ray& r) const{
    return _intersect(root, r);
}

const bool octree::intersectB(const ray& r) const{
}

const intersection octree::leafTest(const octreeNode& node, const ray& r) const{
    // Check each primitive and find the closest intersection.
    primitive* closestPrim;
    ray closestRay;
    intersection closestIsect;
    bool didHit = false;
    float minDist = MAX_FLOAT;

    for(unsigned int i=0; i<node.contents.size(); ++i){
        ray rCopy(r);
        const intersection isect = node.contents[i]->intersect(rCopy);
        if(isect.hit){
            didHit = true;

            if(isect.t < minDist){
                closestPrim = node.contents[i];
                minDist = isect.t;
                closestRay = rCopy;
                closestIsect = isect;
            }
        }
    }

    // If we missed all the primitives in the leaf,
    // return no hit, otherwise return the hitpoint.
    if(didHit){
        return closestIsect;
    }else{
        return noIntersect;
    }
}

const intersection octree::_intersect(const octreeNode* node, ray& r) const{
    if(node->isLeaf){
        return leafTest(*node, r);
    }

    // TODO: Make this test more intelligent.
    arraylist<intersection> isects;
    float tMin = 0, tMax = 0;
    for(unsigned int i=0; i<8; ++i){
        if(node->children[i] && node->children[i]->box.intersect(r, tMin, tMax)){
            const intersection isectC = _intersect(node->children[i], r);
            if(isectC.hit){
                isects.add(isectC);
            }
        }
    }
}

const bool octree::_intersectB(const octreeNode* node, const ray& r) const{
}
