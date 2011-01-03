#pragma once

#include "vector.hpp"
#include "constants.hpp"
#include <limits>

using std::numeric_limits;

class ray {
	public:
		ray() : tMin(EPSILON), tMax(numeric_limits<float>::max())
		{}

		ray(const point3& o, const vec3& d) :
			origin(o), direction(d), invDir(1.f/d), tMin(EPSILON), tMax(numeric_limits<float>::max())
		{}

		ray(const ray& r, const float& tm, const float& tM) :
			origin(r.origin), direction(r.direction), invDir(r.invDir), tMin(tm), tMax(tM)
		{}

		ray(const point3& o, const vec3& d, const float& tm, const float& tM) :
			origin(o), direction(d), invDir(1.f/d), tMin(tm), tMax(tM)
		{}

		point3 origin;
		vec3 direction;
		vec3 invDir;
		float tMin, tMax;
        float etao, etai;
};
