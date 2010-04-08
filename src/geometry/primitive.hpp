#ifndef __RT_PRIM__
#define __RT_PRIM__

#include "aabb.hpp"
#include "intersectable.hpp"
#include "mathlib/point.hpp"

#include <tr1/memory>
using std::tr1::shared_ptr;

class shape;
typedef shared_ptr<shape> shapePtr;

class primitive : public intersectable {
	public:
		primitive(shape* p) : parent(p) {}

		virtual const bool intersect(ray& r) const = 0;
		virtual const vec3 getNormal(const point3& p) const = 0;

		void setParent(shape* p){
			parent.reset(p);
		}

		const aabb& getBounds(){
			return boundingBox;
		}

		shapePtr getParent() const {
			return parent;
		}

	protected:
		shapePtr parent;
		aabb boundingBox;
};

typedef shared_ptr<primitive> primitivePtr;
#endif
