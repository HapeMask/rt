#include "octree.hpp"
#include "intersection.hpp" 

#include "scene/scene.hpp"
#include "geometry/primitive.hpp"
#include "geometry/aabb.hpp"

#include <sys/time.h>
#include <vector>
using std::vector;

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

    cerr << "Building octree..." << endl;
	struct timeval start, end;
	gettimeofday(&start, NULL);

    root = _build(0, s.getBounds(), prims);

	gettimeofday(&end, NULL);
	float sec = end.tv_sec - start.tv_sec;
	sec += (end.tv_usec - start.tv_usec) / 1e6;
	cerr << "Built octree in " << sec << "s." << endl;
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

    // TOOD: Make this less dumb.
    aabb boxes[8];
    for(int i=0; i<8; ++i){
        if(i & 1){
            boxes[i].setFront(box.mid.z);
            boxes[i].setBack(box.max.z);
        }else{
            boxes[i].setFront(box.min.z);
            boxes[i].setBack(box.mid.z);
        }

        if(i & 2){
            boxes[i].setTop(box.max.y);
            boxes[i].setBottom(box.mid.y);
        }else{
            boxes[i].setTop(box.mid.y);
            boxes[i].setBottom(box.min.y);
        }

        if(i & 4){
            boxes[i].setLeft(box.mid.x);
            boxes[i].setRight(box.max.x);
        }else{
            boxes[i].setLeft(box.min.x);
            boxes[i].setRight(box.mid.x);
        }
    }

    for(int i=0;i<8;++i){
        node->children[i] = _build(depth+1, boxes[i], boxPrims);
    }

    return node;
}

const intersection octree::intersect(ray& r) const{
    intersection s = _intersect(root, r);
    cerr << "sh: " << s.hit << endl;
    return s;
}

bool octree::intersectB(const ray& r) const{
    return _intersectB(root, r);
}

const intersection octree::leafTest(const octreeNode& node, const ray& r) const{
    // Check each primitive and find the closest intersection.
    primitive* closestPrim;
    ray closestRay;
    intersection closestIsect;
    bool didHit = false;
    float minDist = MAX_FLOAT;

    for(int i=0; i<node.contents.size(); ++i){
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
    float tMin = 0, tMax = 0;
    const aabb& box = node->box;

    if(!box.intersect(r, tMin, tMax)){
        return noIntersect;
    }else if(node->isLeaf){
        return leafTest(*node, r);
    }

    const float tMid = (tMin+tMax)/2.f;
    point3 p = r.origin + tMin*r.direction;
    intersection isect;
    int curBox, i = 0;

    // Check the boxes that the ray hits in the order that it hits them.
    while(!isect.hit && i < 3){
        curBox = 0;
        if(p.x > box.mid.x){
            curBox |= 4;
        }
        if(p.y > box.mid.y){
            curBox |= 2;
        }
        if(p.z > box.mid.z){
            curBox |= 1;
        }

        isect = _intersect(node->children[curBox], r);

        p += tMid*r.direction;
        ++i;
    }

    return isect;
}

bool octree::_intersectB(const octreeNode* node, const ray& r) const{
    return false;
}
