#ifndef __RT_TRIANGLE__
#define __RT_TRIANGLE__

#include <cmath>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "intersectable.hpp"

class triangle : public intersectable {
	public:
		triangle(const point3& a, const point3& b, const point3& c);

		virtual bool intersect(ray& r, point3& p);

		const point3& a() const {
			return points[0];
		}

		const point3& b() const {
			return points[1];
		}

		const point3& c() const {
			return points[2];
		}

		point3& a(){
			return points[0];
		}

		point3& b(){
			return points[1];
		}

		point3& c(){
			return points[2];
		}

		vec3 normal(){
			return normal_;
		}

	private:
		point3 points[3];
		vec3 normal_;

		vec2 aPrime;
		vec2 bPrime;
		vec2 cPrime;

		unsigned short axis1;
		unsigned short axis2;
};
#endif
