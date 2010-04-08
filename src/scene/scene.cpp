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

void scene::addLight(light* l){
	lightPtr p(l);
	lights.push_back(p);
}

void scene::addShape(shape* s){
	shapePtr p(s);
	shapes.push_back(p);
	needsBuilding = true;
}

void scene::addEmitter(shape* s){
	shapePtr p(s);
	emitters.push_back(p);
	needsBuilding = true;
}

void scene::setAccelerator(accelerator* a){
	accel.reset(a);
}

const intersection scene::intersect(ray& r) const{
	return accel->intersect(r);
}

const bool scene::intersectB(ray& r) const{
	return accel->intersectB(r);
}

void scene::build(){
	if(needsBuilding){
		accel->build(*this);
		needsBuilding = false;
	}
}
