#include "scene.hpp"
#include "geometry/intersectable.hpp"
#include "geometry/shape.hpp"

#include "acceleration/defaultaccelerator.hpp"
#include "acceleration/intersection.hpp"
#include "light/light.hpp"

scene::scene() : accel(new defaultAccelerator()), needsBuilding(false)
{}

scene::scene(accelerator* a) : accel(a), needsBuilding(false)
{}

scene::~scene(){
	delete accel;

	for(size_t i = 0; i<lights.size(); i++){
		delete(lights[i]);
	}

	for(size_t i = 0; i<myShapes.size(); i++){
		delete(myShapes[i]);
	}
}

void scene::addLight(light* l){
	lights.push_back(l);
}

void scene::addPrimitive(primitive* p){
	shapes.push_back(p);
	myShapes.push_back(p);
	needsBuilding = true;
}

void scene::addShape(const shape& s){
	for(size_t i=0; i<s.primitives().size(); i++){
		shapes.push_back(s.primitives()[i]);
	}

	needsBuilding = true;
}

void scene::setAccelerator(accelerator* a){
	delete accel;
	accel = a;
}

const intersection scene::intersect(ray& r) const{
	return accel->intersect(r);
}

const intersection scene::intersect1(ray& r) const{
	return accel->intersect1(r);
}

const bool scene::intersectB(ray& r) const{
	return accel->intersectB(r);
}

void scene::build(){
	if(needsBuilding){
		accel->build(shapes);
		needsBuilding = false;
	}
}
