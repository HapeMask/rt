#ifndef __RT_INTERSECT__
#define __RT_INTERSECT__

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/constants.hpp"

class intersection {
	public:
		intersection() : hit(false), t(POS_INF) {}

		intersection(const bool& didHit) : hit(didHit), t(POS_INF) {}
		intersection(const shape* sh, const primitive* pr) : s(sh), p(pr), hit(true) {}
		intersection(const shape* sh, const primitive* pr, const float& f) : s(sh), p(pr), hit(true), t(f) {}

		const shape* s;
		const primitive* p;
		bool hit;
        float t;
        vec3 dpdu, dpdv, normal, shadingNormal;
};

static const intersection noIntersect(false);
#endif
