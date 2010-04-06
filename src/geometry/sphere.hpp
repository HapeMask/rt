#ifndef __RT_SPHERE__
#define __RT_SPHERE__

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"

class sphere : public primitive {
	public:
		sphere(const point3& p, const float& r, shape* parent = NULL);

		virtual const bool intersect(ray& r) const;
		virtual const vec3 getNormal(const point3& p) const;

	private:
		point3 location;
		float radius, radius2;
};
#endif
