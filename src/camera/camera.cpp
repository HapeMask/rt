#include "camera.hpp"
#include "mathlib/ray.hpp"
#include <cmath>

void camera::getRay(const int& x, const int& y, ray& r){
	float upFrac = (float)(halfH - y) / (float)halfH;
	float sideFrac = (float)(halfW - x) / (float)halfW;
	vec3 v = (left * sideFrac) + (up * upFrac) + forward;
	r.direction() = normalize(v);
	r.origin() = position;
}
