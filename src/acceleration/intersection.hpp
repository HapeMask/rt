#pragma once

#include "geometry/primitive.hpp"
#include "mathlib/constants.hpp"

class light;
class shape;
class intersection {
	public:
		intersection() : hit(false), t(POS_INF) {}
        intersection(const light* l, const float& f = 0.f) : s(NULL), p(NULL), li(l), hit(true), t(f) {}
        intersection(const primitive* pr, const float& f = 0.f) : s(pr->getParent()), p(pr), li(NULL), hit(true), t(f) {}
        explicit intersection(const bool& didHit) : hit(didHit), t(POS_INF) {}

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
