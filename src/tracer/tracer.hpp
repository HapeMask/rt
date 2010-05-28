#pragma once

#include <omp.h>

#include "color/color.hpp"
#include "scene/scene.hpp"

const unsigned int MAXDEPTH = 20;
const unsigned int areaSamples = 16;

//static unsigned int sampleIndex[8];
class rayTracer {
	public:
		rayTracer(scene* p) : parent(p), imgWidth(p->getCamera()->width()),
        imgHeight(p->getCamera()->height())
        {}

		virtual const rgbColor L(const ray& r) const = 0;
        virtual ~rayTracer() {}

	protected:
		scene* parent;
        unsigned int imgWidth, imgHeight;
};

class whittedRayTracer : public rayTracer {
	public:
		whittedRayTracer(scene* p) : rayTracer(p) {}

		virtual const rgbColor L(const ray& r) const;

	private:
		const rgbColor _L(ray& r, const unsigned int& depth = 0) const;
};
