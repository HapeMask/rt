#ifndef __RT_AABB__
#define __RT_AABB__

#include <cmath>
#include <iostream>
using namespace std;

/**
 * Axis-Aligned Bounding Box
 * Sides defined looking down the +Z axis.
 * (Left = minX, Right = maxX, Top = maxY, Bottom = minY,
 * Front = minZ, Back = maxZ)
 */
class aabb {
	public:
		aabb() : top(0), bottom(0),
		left(0), right(0),
		front(0), back(0)
		{}

		aabb(	const float& t,
				const float& b,
				const float& l,
				const float& r,
				const float& f,
				const float& ba) :
			top(t), bottom(b),
			left(l), right(r),
			front(f), back(ba) {}

		const float height(){
			return abs(top - bottom);
		}

		const float width(){
			return abs(left - right);
		}

		const float depth(){
			return abs(front - back);
		}

		float top, bottom,
		left, right,
		front, back;
};

ostream& operator<<(ostream& out, const aabb& b);
#endif
