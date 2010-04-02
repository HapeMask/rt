#ifndef __RT_TRACER__
#define __RT_TRACER__

#include "color/color.hpp"
#include "scene/scene.hpp"

class rayTracer {
	public:
		const color L() const;

	private:
		scene* parent;
};
#endif
