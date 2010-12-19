#include <cmath>
#include <iostream>
using namespace std;

#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"

#ifdef HAVE_SSE2
#include "mathlib/sse.hpp"
#endif

#include "utility.hpp"

const float radians(const float& deg){
	return (deg / 180.f) * PI;
}

const float degrees(const float& rad){
	return (rad / PI) * 180.f;
}

rgbColor clamp(const rgbColor& c){
	rgbColor c1(c);
	c1.r = min(max(c.r, 0.f), 1.f);
	c1.g = min(max(c.g, 0.f), 1.f);
	c1.b = min(max(c.b, 0.f), 1.f);
	return c1;
}

vec3 clamp(const vec3& v){
	vec3 v1(v);
	v1.x = min(max(v.x, 0.f), 1.f);
	v1.y = min(max(v.y, 0.f), 1.f);
	v1.z = min(max(v.z, 0.f), 1.f);
	return v1;
}

float clamp(const float& f){
	return min(max(f, 0.f), 1.f);
}

vec3 reflect(const vec3& v, const vec3& n){
	return v - (2*dot(v,n)*n);
}

const int roundUpToMultiple(int n, int m){
    return ceil((float)n/(float)m) * m;
}

void makeCoordinateSystem(const vec3& u, vec3& v, vec3& w){
    if(fabsf(u.x) > fabsf(u.z)){
        const float invLen = 1.f / sqrtf(u.x*u.x + u.y*u.y);
        v.x = -u.y * invLen;
        v.y = u.x * invLen;
        v.z = 0.f;
        v = normalize(v);
    }else{
        const float invLen = 1.f / sqrtf(u.z*u.z + u.y*u.y);
        v.x = 0.f;
        v.y = -u.z * invLen;
        v.z = u.y * invLen;
    }
    w = normalize(cross(u,v));
}

const vec3 worldToBsdf(const vec3& v, const intersection& isect){
    return vec3(dot(v, isect.binormal), dot(v, isect.shadingNormal), dot(v, isect.tangent));
}

const vec3 bsdfToWorld(const vec3& v, const intersection& isect){
    return vec3(
        isect.binormal.x * v.x + isect.shadingNormal.x * v.y + isect.tangent.x * v.z,
        isect.binormal.y * v.x + isect.shadingNormal.y * v.y + isect.tangent.y * v.z,
        isect.binormal.z * v.x + isect.shadingNormal.z * v.y + isect.tangent.z * v.z);
}

const vec3 worldToBsdf(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent){
    return vec3(dot(v, binormal), dot(v, normal), dot(v, tangent));
}

const vec3 bsdfToWorld(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent){
    return vec3(
        binormal.x * v.x + normal.x * v.y + tangent.x * v.z,
        binormal.y * v.x + normal.y * v.y + tangent.y * v.z,
        binormal.z * v.x + normal.z * v.y + tangent.z * v.z);
}

void sphericalToDirection(vec3& v, const float& sinTheta, const float& cosTheta, const float& phi){
    v.x = sinTheta * cosf(phi);
    v.y = cosTheta;
    v.z = sinTheta * sinf(phi);
}

/**
 * Fast Float/Int conversion from PBRT.
 */
//2^52 * 1.5, uses limited precision to floor.
int Round2Int(double val) {
	val	= val + doublemagic;
	return (reinterpret_cast<long*>(&val))[0];
}

#ifdef HAVE_SSE2
int Float2Int(const float val) {
    return float2int(val);
#else
int Float2Int(const double val) {
	return (val<0) ?  Round2Int(val+doublemagicroundeps) :
		   Round2Int(val-doublemagicroundeps);
#endif
}

int Floor2Int(const double val) {
	return Round2Int(val - doublemagicroundeps);
}

int Ceil2Int(const double val) {
	return Round2Int(val + doublemagicroundeps);
}

/**
 * Computes the double integral of the 2D Gaussian with stddev sigma for X and
 * Y in the range [-0.5, 0.5]
 */
const float gaussian2DNormalization(const float& sigma) {
    return 0.3989422804f * fabs(1.f/sigma) * erf(0.353553389f *
            fabs(1.f/sigma)) * (-1.25331413f * sigma *
            erf(-0.353553389f/sigma) + 1.25331413f * sigma *
            erf(0.353553389f/sigma));
}