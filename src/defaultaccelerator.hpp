#ifndef __RT_DEFAULTACCEL__
#define __RT_DEFAULTACCEL__

#include <vector>
#include "intersectable.hpp"
#include "accelerator.hpp"

using namespace std;

/**
 * Default "acceleration structure." Just checks all
 * available shapes until it finds an intersection.
 */
class defaultAccelerator : public accelerator {
	public:
		bool intersect(ray& r, point3& p);

		void build(const vector<intersectable*> shapes){
			shapes_ = shapes;
		}

	private:
		vector<intersectable*> shapes_;
};

#endif

