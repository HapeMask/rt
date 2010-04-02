#ifndef __INTERS__
#define __INTERS__
#include "mathlib/point.hpp"
#include "mathlib/ray.hpp"

class intersectable {
	public:
		virtual bool intersect(const ray& r, intersection& i) const = 0;
};
#endif
