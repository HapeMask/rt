#ifndef __RT_TRACER__
#define __RT_TRACER__

#include "color/color.hpp"
#include "scene/scene.hpp"

const int MAXDEPTH = 20;
class rayTracer {
	public:
		rayTracer(scene* p) : parent(p) {}
		virtual const rgbColor L(ray& r) const = 0;

	protected:
		scene* parent;
};

class whittedRayTracer : public rayTracer {
	public:
		whittedRayTracer(scene* p) : rayTracer(p) {}
		virtual const rgbColor L(ray& r) const;
	private:
		const rgbColor _L(ray& r, const int& depth = 0) const;
};
#endif
