#ifndef __RT_INTERSECT__
#define __RT_INTERSECT__

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/constants.hpp"

class intersection {
	public:
		intersection() : hit(false), t(POS_INF) {}

		intersection(const bool& didHit) : hit(didHit), t(POS_INF) {}
		intersection(const shapePtr& sh, primitive* pr) : s(sh), p(pr), hit(true) {}
		intersection(const shapePtr& sh, primitive* pr, const float& f) : s(sh), p(pr), hit(true), t(f) {}

		shapePtr s;
		primitive* p;
		bool hit;
        float t;
};
#endif
