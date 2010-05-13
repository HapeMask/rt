#ifndef __RT_PRIM__
#define __RT_PRIM__

#include "samplers/samplers.hpp"
#include "aabb.hpp"
#include "mathlib/point.hpp"

#include <vector>
#include <tr1/memory>
using namespace std;
using tr1::shared_ptr;

class shape;
typedef shared_ptr<shape> shapePtr;
class intersection;

class primitive {
	public:
        primitive(const aabb& box) : boundingBox(box) {}
		virtual ~primitive() {}

		virtual const intersection intersect(ray& r) const = 0;

        virtual const point3 sampleSurface(const float& u0, const float& u1) const = 0;
        virtual const point3 uniformSampleSurface() const {
            return sampleSurface(sampleUniform(), sampleUniform());
        }

        virtual const vec3 getNormal(const point3& p) const = 0;
        virtual const float area() const = 0;

		void setParent(shape* p){
            parent = p;
		}

		const aabb& getBounds() const{
			return boundingBox;
		}

		shape* getParent() const {
			return parent;
		}

	protected:
		shape* parent;
		aabb boundingBox;
};

typedef shared_ptr<primitive> primitivePtr;
#endif
