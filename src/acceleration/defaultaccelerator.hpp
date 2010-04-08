#ifndef __RT_DEFAULTACCEL__
#define __RT_DEFAULTACCEL__

#include <vector>
#include <tr1/memory>

#include "intersection.hpp"
#include "accelerator.hpp"
#include "scene/scene.hpp"
#include "geometry/primitive.hpp"

using namespace std;
using tr1::shared_ptr;

/**
 * Default "acceleration structure." Just checks all
 * available shapes until it finds an intersection.
 */
class defaultAccelerator : public accelerator {
	public:
		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;
		virtual const intersection intersectE(ray& r) const;
		virtual const bool intersectEB(const ray& r) const;

		virtual void build(const scene& s);

	private:
		vector<primitivePtr> shapes;
		vector<primitivePtr> emitters;
};

#endif

