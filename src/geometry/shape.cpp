#include "shape.hpp"
#include "geometry/primitive.hpp"
#include "materials/material.hpp"

shape::shape(const vector<primitivePtr>& pr) : prims(pr)
{}

shape::shape(material* m) : mat(m)
{}

shape::shape(material* m, const vector<primitivePtr>& pr) : prims(pr), mat(m)
{}

void shape::setMaterial(materialPtr m){
	mat = m;
}

void shape::addPrimitive(primitivePtr p){
	prims.push_back(p);
	p->setParent(this);
	const aabb& box = p->getBounds();
	bounds.top = max(bounds.top, box.top);
	bounds.bottom = min(bounds.bottom, box.bottom);
	bounds.right = max(bounds.right, box.right);
	bounds.left = min(bounds.left, box.left);
	bounds.back = max(bounds.back, box.back);
	bounds.front = min(bounds.front, box.front);
}
