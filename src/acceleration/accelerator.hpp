#pragma once

#include "intersection.hpp"
#include "geometry/primitive.hpp"

#include <memory>

const int AXIS_X = 0;
const int AXIS_Y = 1;
const int AXIS_Z = 2;
const int AXIS_LEAF = 4;

class scene;
class accelerator {
	public:
        virtual ~accelerator() {}

		virtual const intersection intersect(ray& r) const = 0;
		virtual bool intersectB(const ray& r) const = 0;

		virtual void build(const scene& s) = 0;
};

typedef std::shared_ptr<accelerator> acceleratorPtr;

inline bool aabbMidCmpX(primitivePtr a, primitivePtr b) { return (a->getBounds().mid.x < b->getBounds().mid.x); }
inline bool aabbMidCmpY(primitivePtr a, primitivePtr b) { return (a->getBounds().mid.y < b->getBounds().mid.y); }
inline bool aabbMidCmpZ(primitivePtr a, primitivePtr b) { return (a->getBounds().mid.z < b->getBounds().mid.z); }

inline bool aabbMinCmpX(primitivePtr a, primitivePtr b) { return (a->getBounds().min.x < b->getBounds().min.x); }
inline bool aabbMinCmpY(primitivePtr a, primitivePtr b) { return (a->getBounds().min.y < b->getBounds().min.y); }
inline bool aabbMinCmpZ(primitivePtr a, primitivePtr b) { return (a->getBounds().min.z < b->getBounds().min.z); }
