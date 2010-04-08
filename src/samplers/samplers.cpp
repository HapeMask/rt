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
	v.z() = sin(theta);
	v *= r;
}

void sampleHemisphere(vec3& v, const float& u0, const float& u1){
	v.y() = u0;
	const float r = sqrt(max(0.f, 1.f - u0*u0));
	const float phi = TWOPI * u1;
	v.x() = r * cos(phi);
	v.z() = r * sin(phi);
}

void cosineSampleHemisphere(vec3& v, const float& u0, const float& u1){
	sampleDisk(v, u0, u1);
	v.y() = sin(acos(sqrt(v.x()*v.x() + v.z()*v.z())));
}
