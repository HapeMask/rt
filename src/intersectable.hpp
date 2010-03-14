#ifndef __INTERS__
#define __INTERS__
#include "point.hpp"
#include "ray.hpp"

class intersectable {
	public:
		virtual bool intersect(ray& r, point3& p) = 0;
};
#endif
