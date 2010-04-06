#include "samplers.hpp"
#include "mathlib/constants.hpp"

#include <cstdlib>
#include <ctime>
#include <cmath>

const float sampleUniform(){
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void sampleDisk(vec3& v, const float& u0, const float& u1){
	const float r = sqrt(u0);
	const float theta = u1 * TWOPI;
	v.x() = cos(theta);
	v.y() = sin(theta);
	v *= r;
}

void sampleHemisphere(vec3& v, const float& u0, const float& u1){
	sampleDisk(v, u0, u1);
	v.z() = sin(acos(sqrt(v.x()*v.x() + v.y()*v.y())));
}
