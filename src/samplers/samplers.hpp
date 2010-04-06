#ifndef __RT_SAMPLERS___
#define __RT_SAMPLERS__

#include "mathlib/vector.hpp"

const float sampleUniform();
void sampleDisk(vec3& v, const float& u0, const float& u1);
void sampleHemisphere(vec3& v, const float& u0, const float& u1);
#endif
