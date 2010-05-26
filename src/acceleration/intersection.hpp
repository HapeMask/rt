#ifndef __RT_INTERSECT__
#define __RT_INTERSECT__

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/constants.hpp"

class light;
class intersection {
	public:
		intersection() : hit(false), t(POS_INF) {}

		explicit intersection(const bool& didHit) : hit(didHit), t(POS_INF) {}
		intersection(const primitive* pr) : s(pr->getParent()), p(pr), li(NULL), hit(true), t(0) {}
		intersection(const primitive* pr, const float& f) : s(pr->getParent()), p(pr), li(NULL), hit(true), t(f) {}
		intersection(const primitive* pr, const light* pl, const float& f) : s(pr->getParent()), p(pr), li(pl), hit(true), t(f) {}

		const shape* s;
		const primitive* p;
        const light* li;
		bool hit;
        float t;
        vec3 normal, binormal, tangent, shadingNormal;
        vec2 uv;
        int debugInfo;
};

static const intersection noIntersect(false);
#endif
