#include "scene.hpp"
#include "../acceleration/defaultaccelerator.hpp"

scene::scene(){
	accel = new defaultAccelerator();
}

scene::scene(accelerator* a){
	accel = a;
}

scene::~scene(){
	delete accel;
}
