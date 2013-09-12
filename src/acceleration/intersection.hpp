#pragma once

#include "geometry/primitive.hpp"
#include "mathlib/constants.hpp"

class light;
class shape;
class intersection {
    public:
        // Delegating Constructors
        intersection(const light* l, const float t = 0.f) :
            intersection(nullptr, nullptr, l, true, t) {}
        intersection(const primitive* p, const float t = 0.f) :
            intersection(p->getParent(), p, nullptr, true, t) {}

        // Full Constructor
        intersection(
                const shape* _s = nullptr,
                const primitive* _p = nullptr,
                const light* _li = nullptr,
                bool _hit = false,
                float _t = POS_INF,
                vec3 _normal=vec3(0.f), vec3 _binormal=vec3(0.f), vec3 _tangent=vec3(0.f), vec3 _shadingNormal=vec3(0.f),
                vec2 _uv=vec2(0.f), int _debugInfo=0) :
            s(_s), p(_p), li(_li),
            hit(_hit), t(_t),
            normal(_normal), binormal(_binormal), tangent(_tangent),
            shadingNormal(_shadingNormal), uv(_uv), debugInfo(_debugInfo) {}

        const shape* s;
        const primitive* p;
        const light* li;
        bool hit;
        float t;
        vec3 normal, binormal, tangent, shadingNormal;
        vec2 uv;
        int debugInfo;
};

static const intersection noIntersect(nullptr, nullptr, nullptr, false);
