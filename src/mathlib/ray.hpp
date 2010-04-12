#ifndef __RAY__
#define __RAY__

#include "point.hpp"
#include "vector.hpp"
#include "constants.hpp"
#include <limits>

class ray {
	public:
		ray() : tMin(EPSILON), tMax(numeric_limits<float>::max())
		{}

		ray(const point3& o, const vec3& d) :
			origin(point3(o)), direction(vec3(d)), tMin(EPSILON), tMax(numeric_limits<float>::max())
		{}

		ray(const point3& o, const vec3& d, const float& tm, const float& tM) :
			origin(point3(o)), direction(vec3(d)), tMin(tm), tMax(tM)
		{}

		point3 origin;
		vec3 direction;
		float tMin, tMax;
};

#endif
