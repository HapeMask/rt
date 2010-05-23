#include "octree.hpp"
#include "intersection.hpp" 

#include "scene/scene.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

void octree::build(const scene& s){
    const vector<shapePtr>& shapes = s.getShapes();

    // Fill the list of primitives.
    for(size_t i=0; i<shapes.size(); ++i){
        const vector<primitivePtr>& p = shapes[i]->getPrimitives();
        for(size_t j=0; j<p.size(); ++j){
            prims.push_back(p[j]);
        }
    }

    root = new octreeNode();
    root->box = s.getBounds();
    _build(root);
}

void octree::_build(octreeNode* node){
}
