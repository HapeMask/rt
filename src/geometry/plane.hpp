#ifndef __RT_PLANE__
#define __RT_PLANE__

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"

class plane : public primitive {
	public:
		plane(const vec3& n, const float& d_, shape* parent = NULL);
		virtual ~plane() {}

		virtual const bool intersect(ray& r) const;
		virtual const vec3 getNormal(const point3& p) const;
        virtual const point3 uniformSampleSurface() const{
            return point3(0,0,0);
        }

	private:
		vec3 normal;
		float d;
};
#endif

