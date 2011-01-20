#pragma once

#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"

#ifdef HAVE_SSE2
#include "mathlib/sse.hpp"
#endif

const float radians(const float& deg);
const float degrees(const float& rad);

const bool isFinite(const float& f);

rgbColor clamp(const rgbColor& c);
vec3 clamp(const vec3& v);
float clamp(const float& f);

vec3 reflect(const vec3& v, const vec3& n);

const int roundUpToMultiple(int n, int m);

void makeCoordinateSystem(const vec3& u, vec3& v, vec3& w);

const vec3 worldToBsdf(const vec3& v, const intersection& isect);
const vec3 bsdfToWorld(const vec3& v, const intersection& isect);
const vec3 worldToBsdf(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent);
const vec3 bsdfToWorld(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent);

void sphericalToDirection(vec3& v, const float& sinTheta, const float& cosTheta, const float& phi);

/**
 * Fast Float/Int conversion from PBRT.
 */
//2^52 * 1.5, uses limited precision to floor.
const double doublemagic = 6755399441055744.0;
const double doublemagicroundeps = .5-1.4e-11;
int Round2Int(double val);
#ifdef HAVE_SSE2
int Float2Int(const float val);
#else
int Float2Int(const double val);
#endif

int Floor2Int(const double val);
int Ceil2Int(const double val);
const float gaussian2DNormalization(const float& sigma);
