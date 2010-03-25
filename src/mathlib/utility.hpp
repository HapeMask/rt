#ifndef __RT_UTILITY__
#define __RT_UTILITY__
#include "constants.hpp"

inline const float radians(const float& deg){
	return (deg / 180.f) * PI;
}

inline const float degrees(const float& rad){
	return (rad / PI) * 180.f;
}

#endif
