#ifndef __RT_SHAPE__
#define __RT_SHAPE__

#include "primitive.hpp"
#include "materials/material.hpp"
#include "color/color.hpp"

#include <vector>
#include <tr1/memory>
using namespace std;
using tr1::shared_ptr;

class shape {
	public:
		void setMaterial(materialPtr m);
		void addPrimitive(primitivePtr p);

		materialPtr getMaterial() const {
			return mat;
		}

		const vector<primitivePtr>& getPrimitives() const {
			return prims;
		}

		const aabb& getBounds(){
			return bounds;
		}

	protected:
		vector<primitivePtr> prims;
		materialPtr mat;
		aabb bounds;
};

typedef shared_ptr<shape> shapePtr;
#endif
