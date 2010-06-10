#pragma once

#include <omp.h>

#include "color/color.hpp"
#include "scene/scene.hpp"

const unsigned int MAXDEPTH = 20;
const unsigned int areaSamples = 16;

//static unsigned int sampleIndex[8];
class rayTracer {
	public:
		rayTracer(scene* p) : parent(p) {}

		virtual const rgbColor L(const ray& r) const = 0;
        virtual ~rayTracer() {}

	protected:
        const rgbColor sampleOneLight(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        const rgbColor sampleAllLights(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        const rgbColor sampleDirect(const point3& p, const vec3& wo, const intersection& isect,
                const bsdf& bsdf, const light& light) const;

		scene* parent;
};

class whittedRayTracer : public rayTracer {
	public:
		whittedRayTracer(scene* p) : rayTracer(p) {}

		virtual const rgbColor L(const ray& r) const;

	private:
		const rgbColor _L(ray& r, const unsigned int& depth = 0) const;
};
