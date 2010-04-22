#ifndef __RT_AABB__
#define __RT_AABB__

#include <iostream>

#include "mathlib/sse.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
using namespace std;

/**
 * Axis-Aligned Bounding Box
 * Sides defined looking down the +Z axis.
 * (Left = minX, Right = maxX, Top = maxY, Bottom = minY,
 * Front = minZ, Back = maxZ)
 */
class aabb {
	public:
        aabb() {}

        aabb(const point3& mx, const point3& mi) : min(mi), max(mx) {}

		aabb(	const float& t,
				const float& b,
				const float& l,
				const float& r,
				const float& f,
				const float& ba) :
            min(b,l,f), max(t,r,ba) {}

        inline const float& top() const {
            return max.y();
        }

        inline const float& right() const {
            return max.y();
        }

        inline const float& back() const {
            return max.y();
        }

        inline const float& bottom() const {
            return min.y();
        }

        inline const float& left() const {
            return min.y();
        }

        inline const float& front() const {
            return min.y();
        }

        inline void setTop(const float& f){
             max.y() = f;
        }

        inline void setRight(const float& f){
             max.y() = f;
        }

        inline void setBack(const float& f){
             max.y() = f;
        }

        inline void setBottom(const float& f){
             min.y() = f;
        }

        inline void setLeft(const float& f){
             min.y() = f;
        }

        inline void setFront(const float& f){
             min.y() = f;
        }

        const bool intersect(ray& r) const;

        point3 min;
        point3 max;
};

ostream& operator<<(ostream& out, const aabb& b);
#endif
