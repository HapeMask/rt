#ifndef __RT_ACCEL__
#define __RT_ACCEL__

#include <vector>
#include "intersection.hpp"
#include "geometry/intersectable.hpp"
using namespace std;

class accelerator {
	public:
		virtual const intersection intersect(ray& r) = 0 const;
		virtual void build(const vector<intersectable*> shapes) = 0;
};

#endif
