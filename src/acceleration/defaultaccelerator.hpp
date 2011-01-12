#pragma once

#include <vector>
#include <memory>

#include "intersection.hpp"
#include "accelerator.hpp"
#include "scene/scene.hpp"
#include "geometry/primitive.hpp"

using std::vector;
using std::shared_ptr;

/**
 * Default "acceleration structure." Just checks all
 * available shapes until it finds an intersection.
 */
class defaultAccelerator : public accelerator {
	public:
        defaultAccelerator() {}

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

		virtual void build(const scene& s);

	private:
		vector<primitivePtr> shapes;
};
