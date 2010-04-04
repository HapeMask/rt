#include "shape.hpp"
#include "geometry/primitive.hpp"
#include "materials/material.hpp"

shape::shape(const vector<primitive*>& pr) : prims(pr)
{}

shape::shape(material* m) : mat(m)
{}

shape::shape(material* m, const vector<primitive*>& pr) : prims(pr), mat(m)
{}

void shape::setMaterial(material* m){
	mat = m;
}

shape::~shape(){
	for(size_t i = 0; i<prims.size(); i++){
		delete prims[i];
	}
}
