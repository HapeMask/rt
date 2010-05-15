#ifndef __RT_TRACER__
#define __RT_TRACER__

#include <omp.h>

#include "color/color.hpp"
#include "scene/scene.hpp"

const unsigned int MAXDEPTH = 20;
const unsigned int areaSamples = 16;

class rayTracer {
	public:
		rayTracer(scene* p) : parent(p) {}
		virtual const rgbColor L(const ray& r) const = 0;
        virtual ~rayTracer() {}

	protected:
		scene* parent;
};

class whittedRayTracer : public rayTracer {
	public:
		whittedRayTracer(scene* p) : rayTracer(p) {}

        virtual ~whittedRayTracer() {}

		virtual const rgbColor L(const ray& r) const;

	private:
		const rgbColor _L(ray& r, const int& depth = 0) const;
};
#endif
