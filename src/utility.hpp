#pragma once

#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"

#include <cmath>
#include <iostream>
using namespace std;

inline const float radians(const float& deg){
	return (deg / 180.f) * PI;
}

inline const float degrees(const float& rad){
	return (rad / PI) * 180.f;
}

inline rgbColor clamp(const rgbColor& c){
	rgbColor c1(c);
	c1.r = min(max(c.r, 0.f), 1.f);
	c1.g = min(max(c.g, 0.f), 1.f);
	c1.b = min(max(c.b, 0.f), 1.f);
	return c1;
}

inline vec3 clamp(const vec3& v){
	vec3 v1(v);
	v1.x() = min(max(v.x(), 0.f), 1.f);
	v1.y() = min(max(v.y(), 0.f), 1.f);
	v1.z() = min(max(v.z(), 0.f), 1.f);
	return v1;
}

inline float clamp(const float& f){
	return min(max(f, 0.f), 1.f);
}

inline vec3 reflect(const vec3& v, const vec3& n){
	return v - (2*dot(v,n)*n);
}

inline const int roundUpToMultiple(int n, int m){
    return ceil((float)n/(float)m) * m;
}

inline void makeCoordinateSystem(const vec3& u, vec3& v, vec3& w){
    if(fabsf(u.x()) > fabsf(u.z())){
        const float invLen = 1.f / sqrtf(u.x()*u.x() + u.y()*u.y());
        v.x() = -u.y() * invLen;
        v.y() = u.x() * invLen;
        v.z() = 0.f;
        v = normalize(v);
    }else{
        const float invLen = 1.f / sqrtf(u.z()*u.z() + u.y()*u.y());
        v.x() = 0.f;
        v.y() = -u.z() * invLen;
        v.z() = u.y() * invLen;
    }
    w = normalize(cross(u,v));
}

inline const vec3 worldToBsdf(const vec3& v, const intersection& isect){
    return vec3(dot(v, isect.binormal), dot(v, isect.shadingNormal), dot(v, isect.tangent));
}

inline const vec3 bsdfToWorld(const vec3& v, const intersection& isect){
    return vec3(
        isect.binormal.x() * v.x() + isect.shadingNormal.x() * v.y() + isect.tangent.x() * v.z(),
        isect.binormal.y() * v.x() + isect.shadingNormal.y() * v.y() + isect.tangent.y() * v.z(),
        isect.binormal.z() * v.x() + isect.shadingNormal.z() * v.y() + isect.tangent.z() * v.z());
}

/**
 * Fast Float/Int conversion from PBRT.
 */
const double doublemagic = 6755399441055744.0;
const double doublemagicroundeps = .5-1.4e-11;
inline int Round2Int(double val) {
	//2^52 * 1.5, uses limited precision to floor.
	val	= val + doublemagic;
	return (reinterpret_cast<long*>(&val))[0];
}

inline int Float2Int(const double val) {
	return (val<0) ?  Round2Int(val+doublemagicroundeps) :
		   Round2Int(val-doublemagicroundeps);
}

inline int Floor2Int(const double val) {
	return Round2Int(val - doublemagicroundeps);
}

inline int Ceil2Int(const double val) {
	return Round2Int(val + doublemagicroundeps);
}

inline void debugPrint(string s){
#ifdef DEBUG
	cout << s << endl;
#endif
}
