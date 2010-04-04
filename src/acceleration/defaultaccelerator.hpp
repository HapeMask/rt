#ifndef __RT_DEFAULTACCEL__
#define __RT_DEFAULTACCEL__

#include <vector>
#include "geometry/primitive.hpp"
#include "intersection.hpp"
#include "accelerator.hpp"

using namespace std;

/**
 * Default "acceleration structure." Just checks all
 * available shapes until it finds an intersection.
 */
class defaultAccelerator : public accelerator {
	public:
		virtual const intersection intersect(ray& r) const;
		virtual const intersection intersect1(ray& r) const;
		virtual const bool intersectB(ray& r) const;

		virtual void build(const vector<primitive*> shapes);

	private:
		vector<primitive*> contents_;
};

#endif

