#ifndef __RT_UTILITY__
#define __RT_UTILITY__

#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"

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

inline const vec3 worldToBsdf(const vec3& v, const vec3& n, const vec3& dpdu, const vec3& dpdv){
    return vec3(dot(v, dpdu), dot(v, n), dot(v, dpdv));
}

inline const vec3 bsdfToWorld(const vec3& v, const vec3& n, const vec3& dpdu, const vec3& dpdv){
    return vec3(
            dpdu.x() * v.x() + n.x() * v.y() + dpdv.x() * v.z(),
            dpdu.y() * v.x() + n.y() * v.y() + dpdv.y() * v.z(),
            dpdu.z() * v.x() + n.z() * v.y() + dpdv.z() * v.z());
}

inline const float bsdfCosTheta(const vec3& v){
    return v.y();
}

inline const float bsdfCos2Theta(const vec3& v){
    return v.y()*v.y();
}

inline const float bsdfSinTheta(const vec3& v){
    // sintheta(v) = sqrt(1 - cos2theta(v))
    return sqrtf(max(0.f, 1.f - v.y()*v.y()));
}

inline const float bsdfSin2Theta(const vec3& v){
    const float c = bsdfCosTheta(v);
    return 1.f - c*c;
}

inline void debugPrint(string s){
#ifdef DEBUG
	cout << s << endl;
#endif
}

#endif
