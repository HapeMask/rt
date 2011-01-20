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

		aabb(	const float& t,
				const float& b,
				const float& l,
				const float& r,
				const float& f,
				const float& ba) :
            min(b,l,f), max(t,r,ba), mid((min+max)/2.f) {}

        inline const float& top() const {
            return max.y;
        }

        inline const float& right() const {
            return max.x;
        }

        inline const float& back() const {
            return max.z;
        }

        inline const float& bottom() const {
            return min.y;
        }

        inline const float& left() const {
            return min.x;
        }

        inline const float& front() const {
            return min.z;
        }

        inline void setTop(const float& f){
             max.y = f;
             updateMid();
        }

        inline void setRight(const float& f){
             max.y = f;
             updateMid();
        }

        inline void setBack(const float& f){
             max.y = f;
             updateMid();
        }

        inline void setBottom(const float& f){
             min.y = f;
             updateMid();
        }

        inline void setLeft(const float& f){
             min.y = f;
             updateMid();
        }

        inline void setFront(const float& f){
             min.y = f;
             updateMid();
        }

        inline void setMax(const float& f, int axis){
            max(axis) = f;
            updateMid();
        }

        inline void setMin(const float& f, int axis){
            min(axis) = f;
            updateMid();
        }

        bool intersect(const ray& r, float& tmin, float& tmax) const;
        bool intersect(const aabb& box) const;

        vec3 min, max, mid;

    private:
        inline void updateMid() {
             mid = (min+max)/2.f;
        }
};

ostream& operator<<(ostream& out, const aabb& b);

/*
 * Returns the bounding box that bounds boxes a and b.
 */
inline const aabb mergeAabb(const aabb& a, const aabb& b){
    return aabb(min(a.min, b.min), max(a.max, b.max));
}
