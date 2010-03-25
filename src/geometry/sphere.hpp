#ifndef __RT_SPHERE__
#define __RT_SPHERE__

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "intersectable.hpp"

class sphere : public intersectable {
	public:
		sphere(const point3& p, const float& r);

		virtual bool intersect(ray& r, point3& p);

	private:
		point3 location;
		float radius, radius2;
};
#endif
