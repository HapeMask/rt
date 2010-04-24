#include <cmath>

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

    bounds = mergeAabb(bounds, p->getBounds());
}
