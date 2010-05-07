#ifndef __RT_PRIM__
#define __RT_PRIM__

#include "aabb.hpp"
#include "mathlib/point.hpp"

#include <vector>
#include <tr1/memory>
using namespace std;
using tr1::shared_ptr;

class shape;
typedef shared_ptr<shape> shapePtr;

class primitive{
	public:
        primitive(const aabb& box) : boundingBox(box) {}
		virtual ~primitive() {}

		virtual const bool intersect(ray& r) const = 0;
		virtual const vec3 getNormal(const point3& p) const = 0;
        virtual const point3 uniformSampleSurface() const = 0;

		void setParent(shape* p){
            parent = p;
		}

		const aabb& getBounds() const{
			return boundingBox;
		}

        /**
         * NOTE: Never construct a primitivePtr from this!
         * NEVER!
         * This shape* probably came from a shape instance owned by a
         * reference counted shapePtr.
         */
		shape* getParent() const {
			return parent;
		}

	protected:
		shape* parent;
		aabb boundingBox;
};

typedef shared_ptr<primitive> primitivePtr;
#endif
