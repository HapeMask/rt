#ifndef __RT_ACCEL__
#define __RT_ACCEL__

#include <vector>
#include "intersection.hpp"
#include "geometry/primitive.hpp"
using namespace std;

class accelerator {
	public:
		virtual const intersection intersect(ray& r) const = 0;
		virtual const bool intersectB(ray& r) const = 0;

		virtual void build(const vector<primitive*> shapes) = 0;
};

#endif
