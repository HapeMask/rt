#ifndef __RT_SHAPE__
#define __RT_SHAPE__

#include "intersectable.hpp"
#include "materials/material.hpp"
#include "color/color.hpp"

class shape : public intersectable {
	public:
	protected:
		material* mat;
};

#endif
