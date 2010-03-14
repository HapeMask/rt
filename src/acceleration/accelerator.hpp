#ifndef __RT_ACCEL__
#define __RT_ACCEL__

#include <vector>
#include "geometry/intersectable.hpp"
using namespace std;

class accelerator : public intersectable {
	public:
		virtual bool intersect(ray& r, point3& p) = 0;
		virtual void build(const vector<intersectable*> shapes) = 0;
};

#endif
