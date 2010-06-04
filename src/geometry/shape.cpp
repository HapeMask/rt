#include <cmath>

#include "shape.hpp"
#include "geometry/primitive.hpp"
#include "materials/material.hpp"

void shape::setMaterial(materialPtr m){
	mat = m;
}

void shape::addPrimitive(primitivePtr p){
	p->setParent(this);
	prims.push_back(p);

    bounds = mergeAabb(bounds, p->getBounds());
    area_ += p->area();
    pdf_ = 1.f / area_;
}

void shape::setMaterial(material* m){
    mat.reset(m);
}

void shape::addPrimitive(primitive* p){
    primitivePtr pp(p);

	pp->setParent(this);
    prims.push_back(pp);

    bounds = mergeAabb(bounds, p->getBounds());
    area_ += p->area();
    pdf_ = 1.f / area_;
}
