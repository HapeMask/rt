#include "scene.hpp"
#include "geometry/shape.hpp"
#include "acceleration/defaultaccelerator.hpp"

scene::scene() : accel(new defaultAccelerator()), needsBuilding(false), totalVertices(0)
{}

scene::scene(accelerator* a) : accel(a), needsBuilding(false), totalVertices(0)
{}

void scene::addLight(lightPtr l){
	lights.push_back(l);
}

void scene::addLight(light* l){
    lightPtr p(l);
	lights.push_back(p);
}

void scene::addShape(shapePtr s){
    shapes.push_back(s);
    if(s->getMaterial()->isEmissive()){
        emitters.push_back(s);
    }

	needsBuilding = true;
    bounds = mergeAabb(bounds, s->getBounds());

    totalVertices += s->vertexCount();
}

void scene::addShape(shape* s){
    shapePtr p(s);
    shapes.push_back(p);
    if(s->getMaterial()->isEmissive()){
        emitters.push_back(p);
    }

	needsBuilding = true;
    bounds = mergeAabb(bounds, p->getBounds());

    totalVertices += s->vertexCount();
}

void scene::setAccelerator(acceleratorPtr a){
    accel = a;
}

const intersection scene::intersect(ray& r) const{
    const ray rCopy(r);
	const intersection isectG = accel->intersect(r);
    intersection closestIsect = isectG;

    for(int i=0; i<numLights(); ++i){
        const intersection isectL = lights[i]->intersect(rCopy);

        if(isectL.hit && isectL.t <= closestIsect.t){
            closestIsect = isectL;
        }
    }

    return closestIsect;
}

const bool scene::intersectB(const ray& r) const{
	return accel->intersectB(r);
}

void scene::build(){
	if(needsBuilding){
		accel->build(*this);
		needsBuilding = false;
	}
}
