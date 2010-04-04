#ifndef __RT_PRIM__
#define __RT_PRIM__

#include "shape.hpp"
#include "intersectable.hpp"

class primitive : public intersectable {
	public:
		primitive(shape* parent) : parent_(parent) {}

		virtual const bool intersect(ray& r) const = 0;

		shape* parent() const {
			return parent_;
		}

	protected:
		shape* parent_;
};
#endif
