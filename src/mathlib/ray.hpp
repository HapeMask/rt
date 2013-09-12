#pragma once

#include "vector.hpp"
#include "point.hpp"
#include "constants.hpp"
#include <limits>

using std::numeric_limits;

class ray {
    public:
        ray() : ray(point3(0.f), vec3(1.f)) {}

        ray(const ray r, const float tm, const float tM) :
            ray(r.origin, r.direction, tm, tM) {}

        ray(const point3 o, const vec3 d, const float tm = EPSILON, const float& tM = numeric_limits<float>::max(), const float eo = -1.f, const float ei = -1.f) :
            origin(o), direction(d), invDir(1.f/d), tMin(tm), tMax(tM), etao(eo), etai(ei)
        {}

        point3 origin;
        vec3 direction;
        vec3 invDir;
        float tMin, tMax;
        float etao, etai;
};
