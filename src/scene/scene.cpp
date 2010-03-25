#include "scene.hpp"
#include "acceleration/defaultaccelerator.hpp"

scene::scene() : accel(new defaultAccelerator()), needsBuilding(false)
{}

scene::scene(accelerator* a) : accel(a), needsBuilding(false)
{}

scene::~scene(){
	delete accel;
	for(size_t i=0; i<shapes.size(); i++){
		delete shapes[i];
	}
}

void scene::addShape(intersectable* p){
	shapes.push_back(p);
	needsBuilding = true;
}

void scene::setAccelerator(accelerator* a){
	delete accel;
	accel = a;
}

bool scene::intersect(ray& r, point3& p){
	return accel->intersect(r, p);
}

void scene::build(){
	if(needsBuilding){
		accel->build(shapes);
		needsBuilding = false;
	}
}
