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

inline void clamp(rgbColor& c){
	c.r = min(max(c.r, 0.f), 1.f);
	c.g = min(max(c.g, 0.f), 1.f);
	c.b = min(max(c.b, 0.f), 1.f);
}

inline void clamp(vec3& v){
	v.x() = min(max(v.x(), 0.f), 1.f);
	v.y() = min(max(v.y(), 0.f), 1.f);
	v.z() = min(max(v.z(), 0.f), 1.f);
}

inline void debugPrint(string s){
#ifdef DEBUG
	cout << s << endl;
#endif
}

#endif
