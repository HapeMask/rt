#pragma once

#include "mathlib/sse.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/vector.hpp"

/**
 * Axis-Aligned Bounding Box
 * Sides defined looking down the +Z axis.
 * (Left = minX, Right = maxX, Top = maxY, Bottom = minY,
 * Front = minZ, Back = maxZ)
 */
class aabb {
    public:
        aabb(const aabb& a) : min(a.min), max(a.max), mid(a.mid)
        {}

        aabb() :
            min(vec3(MAX_FLOAT)),
            max(vec3(MIN_FLOAT)),
            mid(vec3(0.f)) {}

        aabb(const vec3& mi, const vec3& mx) : min(mi), max(mx), mid((mi+mx)/2.f) {}

        aabb(const float& t, const float& b, const float& l,
             const float& r, const float& f, const float& ba) :
            min(l,b,f), max(r,t,ba), mid((min+max)/2.f) {}

        float left() const { return min.x; }
        float right() const { return max.x; }

        float bottom() const { return min.y; }
        float top() const { return max.y; }

        float front() const { return min.z; }
        float back() const { return max.z; }

        float surfaceArea() const {
            const vec3 diff = max - min;
            return 
                ((abs(diff.x) > EPSILON) ? diff.x : 1) *
                ((abs(diff.y) > EPSILON) ? diff.y : 1) *
                ((abs(diff.z) > EPSILON) ? diff.z : 1);
        }

        bool intersect(const ray& r, float& tmin, float& tmax) const;
        bool intersect(const aabb& box) const;

        vec3 min, max, mid;
};

ostream& operator<<(ostream& out, const aabb& b);

/*
 * Returns the bounding box that bounds boxes a and b.
 */
inline const aabb mergeAabb(const aabb& a, const aabb& b){
    return aabb(min(a.min, b.min), max(a.max, b.max));
}
