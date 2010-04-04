#ifndef __RT_INTERSECT__
#define __RT_INTERSECT__

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"

class intersection {
	public:
		intersection() : s(NULL), p(NULL), hit(false) {}

		intersection(const bool& didHit){
			hit = didHit;
		}

		intersection(shape* sh, primitive* pr) {
			s = sh;
			p = pr;
			hit = true;
		}

		shape* s;
		primitive* p;
		bool hit;
};
#endif
