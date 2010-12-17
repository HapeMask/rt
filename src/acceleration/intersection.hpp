#pragma once

#include "geometry/shape.hpp"
#include "geometry/primitive.hpp"
#include "mathlib/constants.hpp"

class light;
class intersection {
	public:
		intersection() : hit(false), t(POS_INF), uv(-1, -1) {}

		explicit intersection(const bool& didHit) : hit(didHit), t(POS_INF), uv(-1, -1) {}
        intersection(const light* l, const float& f) : s(NULL), p(NULL), li(l), hit(true), t(f), uv(-1, -1) {}
		intersection(const primitive* pr) : s(pr->getParent()), p(pr), li(NULL), hit(true), t(0), uv(-1, -1) {}
		intersection(const primitive* pr, const float& f) : s(pr->getParent()), p(pr), li(NULL), hit(true), t(f), uv(-1, -1) {}
		intersection(const primitive* pr, const light* pl, const float& f) : s(pr->getParent()), p(pr), li(pl), hit(true), t(f), uv(-1, -1) {}

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
