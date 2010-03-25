#ifndef __RT_PLANE__
#define __RT_PLANE__

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "intersectable.hpp"

class plane : public intersectable {
	public:
		plane(const vec3& n, const point3& p0);

		virtual bool intersect(ray& r, point3& p);

	private:
		vec3 normal;
		point3 p0;
};
#endif

