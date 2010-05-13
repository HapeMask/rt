#ifndef __RT_SPHERE__
#define __RT_SPHERE__

#include "mathlib/constants.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "acceleration/intersection.hpp"
#include "primitive.hpp"

class sphere : public primitive {
	public:
		sphere(const point3& p, const float& r);
		virtual ~sphere() {}

		virtual const intersection intersect(ray& r) const;
        virtual const point3 sampleSurface(const float& u0, const float& u1) const;
        virtual const vec3 getNormal(const point3& p) const;

        inline virtual const float area() const {
            return 4.f * PI * radius2;
        }

	private:
		point3 location;
		float radius, radius2;
};
#endif
