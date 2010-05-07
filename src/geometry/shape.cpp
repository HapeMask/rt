#include <cmath>

#include "shape.hpp"
#include "geometry/primitive.hpp"
#include "materials/material.hpp"

void shape::setMaterial(materialPtr m){
	mat = m;
}

void shape::addPrimitive(primitivePtr p){
	prims.push_back(p);
	p->setParent(this);

    bounds = mergeAabb(bounds, p->getBounds());
}
