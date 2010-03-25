#ifndef __RT_UTILITY__
#define __RT_UTILITY__

#include "mathlib/constants.hpp"
#include <iostream>
using namespace std;

inline const float radians(const float& deg){
	return (deg / 180.f) * PI;
}

inline const float degrees(const float& rad){
	return (rad / PI) * 180.f;
}

inline void debugPrint(string s){
#ifdef DEBUG
	cout << s << endl;
#endif
}

#endif
