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

inline void debugPrint(string s){
#ifdef DEBUG
	cout << s << endl;
#endif
}

#endif
