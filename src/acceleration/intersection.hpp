#ifndef __RT_INTERSECT__
#define __RT_INTERSECT__

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"

class intersection {
	public:
		intersection(shape* sh, primitive* pr) {
			s = sh;
			p = pr;
		}

		shape* s;
		primitive* p;
};
#endif
