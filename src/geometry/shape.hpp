#ifndef __RT_SHAPE__
#define __RT_SHAPE__

#include "materials/material.hpp"
#include "color/color.hpp"

#include <vector>
using namespace std;

class primitive;
class shape {
	public:
		shape() {}
		shape(material* m);
		shape(const vector<primitive*>& pr);
		shape(material* m, const vector<primitive*>& pr);
		~shape();

		void setMaterial(material* m);
		void addPrimitive(primitive* p);

		const material* getMaterial() const {
			return mat;
		}

		const vector<primitive*>& primitives() const {
			return prims;
		}

	protected:
		vector<primitive*> prims;
		material* mat;
};

#endif
