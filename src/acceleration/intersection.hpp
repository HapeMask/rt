#ifndef __RT_INTERSECT__
#define __RT_INTERSECT__

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"

class intersection {
	public:
		intersection() : hit(false) {}

		intersection(const bool& didHit){
			hit = didHit;
		}

		intersection(shapePtr sh, primitivePtr pr) {
			s = sh;
			p = pr;
			hit = true;
		}

		shapePtr s;
		primitivePtr p;
		bool hit;
};
#endif
