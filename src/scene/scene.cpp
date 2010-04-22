#include "scene.hpp"
#include "geometry/intersectable.hpp"
#include "geometry/shape.hpp"

#include "camera/camera.hpp"

#include "acceleration/defaultaccelerator.hpp"
#include "acceleration/intersection.hpp"
#include "light/light.hpp"

scene::scene() : accel(new defaultAccelerator()), needsBuilding(false)
{}

scene::scene(accelerator* a) : accel(a), needsBuilding(false)
{}

void scene::addLight(lightPtr l){
	lights.push_back(l);
}

void scene::addShape(shapePtr s){
	shapes.push_back(s);
	needsBuilding = true;
	const aabb& box = s->getBounds();
	bounds.setTop(max(bounds.top(), box.top()));
	bounds.setBottom(min(bounds.bottom(), box.bottom()));
	bounds.setRight(max(bounds.right(), box.right()));
	bounds.setLeft(min(bounds.left(), box.left()));
	bounds.setBack(max(bounds.back(), box.back()));
	bounds.setFront(min(bounds.front(), box.front()));
}

void scene::addEmitter(shapePtr s){
	emitters.push_back(s);
	needsBuilding = true;
}

void scene::setAccelerator(accelerator* a){
	accel.reset(a);
}

const intersection scene::intersect(ray& r) const{
	return accel->intersect(r);
}

const bool scene::intersectB(const ray& r) const{
	return accel->intersectB(r);
}

const intersection scene::intersectE(ray& r) const{
	return accel->intersectE(r);
}

const bool scene::intersectEB(const ray& r) const{
	return accel->intersectEB(r);
}

void scene::setCamera(cameraPtr p){
	cam = p;
}

void scene::build(){
	if(needsBuilding){
		accel->build(*this);
		needsBuilding = false;
	}
}
