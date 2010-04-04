#ifndef __RT_PLANE__
#define __RT_PLANE__

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"

class plane : public primitive {
	public:
		plane(const vec3& n, const point3& p0, shape* parent = NULL);

		virtual const bool intersect(ray& r) const;
		virtual const vec3 getNormal() const;

	private:
		vec3 normal;
		point3 p0;
};
#endif

