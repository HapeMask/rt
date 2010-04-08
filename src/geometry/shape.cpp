#include "shape.hpp"
#include "geometry/primitive.hpp"
#include "materials/material.hpp"

shape::shape(const vector<primitivePtr>& pr) : prims(pr)
{}

shape::shape(material* m) : mat(m)
{}

shape::shape(material* m, const vector<primitivePtr>& pr) : prims(pr), mat(m)
{}

void shape::setMaterial(material* m){
	mat.reset(m);
}

void shape::addPrimitive(primitive* p){
	primitivePtr pp(p);
	prims.push_back(pp);
	p->setParent(this);
}
