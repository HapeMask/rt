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
		shape() {}
		shape(material* m);
		shape(const vector<primitivePtr>& pr);
		shape(material* m, const vector<primitivePtr>& pr);
		virtual ~shape() {}

		void setMaterial(materialPtr m);
		void addPrimitive(primitivePtr p);

		materialPtr getMaterial() const {
			return mat;
		}

		const vector<primitivePtr>& getPrimitives() const {
			return prims;
		}

	protected:
		vector<primitivePtr> prims;
		materialPtr mat;
};

typedef shared_ptr<shape> shapePtr;
#endif
