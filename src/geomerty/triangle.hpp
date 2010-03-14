#ifndef __TRIANGLE__
#define __TRIANGLE__

#include <cmath>

#include "../mathlib/point.hpp"
#include "../mathlib/vector.hpp"
#include "../mathlib/ray.hpp"
#include "intersectable.hpp"

class triangle : public intersectable{
	public:
		triangle(const point3& a, const point3& b, const point3& c){
			points[0] = a;
			points[1] = b;
			points[2] = c;

			normal_ = normalize(cross(b - a, c - a));

			// Find the biggest normal axis, then remove that
			// coordinate from A, B, C to get a,b,cPrime.
			const float maxD = max(max(normal_(0), normal_(1)), normal_(2));
			if(maxD == normal_(0)){
				axis1 = 1;
				axis2 = 2;
			}else if(maxD == normal_(1)){
				axis1 = 0;
				axis2 = 2;
			}else{
				axis1 = 0;
				axis2 = 1;
			}

			aPrime = vec2(a(axis1), a(axis2));
			bPrime = vec2(b(axis1), b(axis2)) - aPrime;
			cPrime = vec2(c(axis1), c(axis2)) - aPrime;
		}

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
