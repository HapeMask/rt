#ifndef __RT_SAMPLERS__
#define __RT_SAMPLERS__

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/constants.hpp"
#include "geometry/triangle.hpp"

#include <cstdlib>
#include <cmath>

inline const float sampleUniform(){
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

inline const int sampleRange(const int& a, const int& b){
    return a + (rand() % (b-a+1));
}

inline void uniformSampleDisk(vec3& v){
    const float u0 = sampleUniform();
    const float u1 = sampleUniform();
	const float r = sqrt(u0);
	const float theta = u1 * TWOPI;
	v.x() = cos(theta) * r;
	v.z() = sin(theta) * r;
}

inline void uniformSampleHemisphere(vec3& v){
    const float u0 = sampleUniform();
    const float u1 = sampleUniform();

	v.y() = u0;
	const float r = sqrt(max(0.f, 1.f - u0*u0));
	const float phi = TWOPI * u1;
	v.x() = r * cos(phi);
	v.z() = r * sin(phi);
}

inline void uniformSampleTriangle(point3& p, const triangle& t){
    const float u0 = sampleUniform();
    const float u1 = sampleUniform();

    const float su0 = sqrtf(u0);
    const float u = 1.f - su0;
    const float v = u1 * su0;

    p = (u * t.a() + v * t.b() + (1.f - u - v) * t.c());
}

inline void cosineSampleHemisphere(vec3& v){
	uniformSampleDisk(v);
	v.y() = sin(acos(sqrt(v.x()*v.x() + v.z()*v.z())));
}
#endif
