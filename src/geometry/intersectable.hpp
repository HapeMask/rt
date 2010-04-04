#ifndef __INTERS__
#define __INTERS__
#include "mathlib/ray.hpp"

class intersectable {
	public:
		virtual const bool intersect(ray& r) const = 0;
};
#endif
