#ifndef __INTERS__
#define __INTERS__
#include "../mathlib/point.hpp"
#include "../mathlib/ray.hpp"

class intersectable {
	public:
		virtual bool intersect(ray& r, point3& p) = 0;
};
#endif
