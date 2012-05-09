#include "scene.hpp"
#include "geometry/shape.hpp"
#include "acceleration/defaultaccelerator.hpp"

scene::scene() : accel(new defaultAccelerator()), needsBuilding(false), totalVertices(0)
{}

scene::scene(accelerator* a) : accel(a), needsBuilding(false), totalVertices(0)
{}

void scene::addLight(light* l){
	lights.push_back(unique_ptr<light>(l));
}

void scene::addShape(shape* s){
    shared_ptr<shape> p(s);
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

    for(const auto& light : lights){
        const intersection isectL = light->intersect(rCopy);

        if(isectL.hit && isectL.t <= closestIsect.t){
            closestIsect = isectL;
        }
    }

    return closestIsect;
}

bool scene::intersectB(const ray& r) const{
	return accel->intersectB(r);
}

void scene::build(){
	if(needsBuilding){
		accel->build(*this);
		needsBuilding = false;
	}
}

void scene::dumpToVbo(GLfloat* vertexBuffer, GLfloat* normalBuffer) const {
    GLfloat* pv = vertexBuffer;
    GLfloat* pn = normalBuffer;

    for(const auto& shape : shapes){
        shape->prepGL(pv, pn);
    }
}

void scene::drawGL() const {
    for(const auto& shape : shapes){
        shape->drawGL();
    }
}
