#ifndef __RT_ACCEL__
#define __RT_ACCEL__

#include "intersection.hpp"
#include "geometry/primitive.hpp"

#include <vector>
#include <tr1/memory>

using namespace std;
using tr1::shared_ptr;

enum AXIS{AXIS_X=0,AXIS_Y=1,AXIS_Z=2,AXIS_LEAF=3};

class scene;
class accelerator {
	public:
        virtual ~accelerator() {}

		virtual const intersection intersect(ray& r) const = 0;
		virtual const bool intersectB(const ray& r) const = 0;

		virtual void build(const scene& s) = 0;
};

typedef shared_ptr<accelerator> acceleratorPtr;

inline bool aabbCmpX(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().x() < b->getBounds().mid().x()); }
inline bool aabbCmpY(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().y() < b->getBounds().mid().y()); }
inline bool aabbCmpZ(primitivePtr a, primitivePtr b) { return (a->getBounds().mid().z() < b->getBounds().mid().z()); }
#endif
