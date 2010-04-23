#ifndef __RT_ACCEL__
#define __RT_ACCEL__

#include "intersection.hpp"
#include "geometry/primitive.hpp"

#include <vector>
#include <tr1/memory>

using namespace std;
using tr1::shared_ptr;

class scene;
class accelerator {
	public:
        virtual ~accelerator() {}

		virtual const intersection intersect(ray& r) const = 0;
		virtual const bool intersectB(const ray& r) const = 0;
		virtual const intersection intersectE(ray& r) const = 0;
		virtual const bool intersectEB(const ray& r) const = 0;

		virtual void build(const scene& s) = 0;
};

typedef shared_ptr<accelerator> acceleratorPtr;
#endif
