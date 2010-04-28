#ifndef __RT_PRIM__
#define __RT_PRIM__

#include "aabb.hpp"
#include "intersectable.hpp"
#include "mathlib/point.hpp"

#include <vector>
#include <tr1/memory>
using namespace std;
using tr1::shared_ptr;

class shape;
typedef shared_ptr<shape> shapePtr;

class primitive : public intersectable {
	public:
		primitive(shape* p) : parent(p) {}
        primitive(shape* p, const aabb& box) : parent(p), boundingBox(box) {}
		virtual ~primitive() {}

		virtual const bool intersect(ray& r) const = 0;
		virtual const vec3 getNormal(const point3& p) const = 0;

		void setParent(shape* p){
			parent.reset(p);
		}

		const aabb& getBounds() const{
			return boundingBox;
		}

		const shapePtr getParent() const {
			return parent;
		}

	protected:
		shapePtr parent;
		aabb boundingBox;
};

typedef shared_ptr<primitive> primitivePtr;
#endif
