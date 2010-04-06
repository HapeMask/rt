#ifndef __RT_PRIM__
#define __RT_PRIM__

#include "shape.hpp"
#include "aabb.hpp"
#include "intersectable.hpp"
#include "mathlib/point.hpp"

class primitive : public intersectable {
	public:
		primitive(shape* parent) : parent_(parent) {}

		void setParent(shape* parent){
			parent_ = parent;
		}

		virtual const bool intersect(ray& r) const = 0;
		virtual const vec3 getNormal(const point3& p) const = 0;

		const aabb& getBounds(){
			return boundingBox;
		}

		shape* parent() const {
			return parent_;
		}

	protected:
		shape* parent_;
		aabb boundingBox;
};
#endif
