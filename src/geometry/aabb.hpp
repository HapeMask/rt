#ifndef __RT_AABB__
#define __RT_AABB__

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
        aabb() : _min(vec3(POS_INF)), _max(vec3(NEG_INF)) {}

        aabb(const vec3& mi, const vec3& mx) : _min(mi), _max(mx), _mid((_min+_max)/2.f) {}

		aabb(	const float& t,
				const float& b,
				const float& l,
				const float& r,
				const float& f,
				const float& ba) :
            _min(b,l,f), _max(t,r,ba), _mid((_min+_max)/2.f) {}

        inline const float& top() const {
            return _max.y();
        }

        inline const float& right() const {
            return _max.x();
        }

        inline const float& back() const {
            return _max.z();
        }

        inline const float& bottom() const {
            return _min.y();
        }

        inline const float& left() const {
            return _min.x();
        }

        inline const float& front() const {
            return _min.z();
        }

        inline const vec3& min() const {
            return _min;
        }

        inline const vec3& max() const {
            return _max;
        }

        inline const vec3& mid() const {
            return _mid;
        }

        inline void setTop(const float& f){
             _max.y() = f;
             updateMid();
        }

        inline void setRight(const float& f){
             _max.y() = f;
             updateMid();
        }

        inline void setBack(const float& f){
             _max.y() = f;
             updateMid();
        }

        inline void setBottom(const float& f){
             _min.y() = f;
             updateMid();
        }

        inline void setLeft(const float& f){
             _min.y() = f;
             updateMid();
        }

        inline void setFront(const float& f){
             _min.y() = f;
             updateMid();
        }

        const bool intersect(const ray& r, float& tmin, float& tmax) const;

    private:
        inline void updateMid() {
             _mid = (_min+_max)/2.f;
        }
        vec3 _min;
        vec3 _max;
        vec3 _mid;
};

ostream& operator<<(ostream& out, const aabb& b);

/*
 * Returns the bounding box that bounds boxes a and b.
 */
inline const aabb mergeAabb(const aabb& a, const aabb& b){
    return aabb(vec3(
                minps(a.min().getSIMD(), b.min().getSIMD())),
            vec3(
                maxps(a.max().getSIMD(), b.max().getSIMD())));
}

#endif
