#ifndef __RT_PATHTRACER__
#define __RT_PATHTRACER__

#include "color/color.hpp"
#include "mathlib/ray.hpp"
#include "scene/scene.hpp"

#include "tracer.hpp"

class pathTracer : public rayTracer {
	public:
		pathTracer(scene* p) : rayTracer(p) {}
		virtual const rgbColor L(const ray& r) const;

	private:
		const rgbColor _L(ray& r, const int& depth = 0) const;
        const rgbColor sampleOneLight(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        const rgbColor sampleDirect(const point3& p, const vec3& wo, const intersection& isect,
                const bsdf& bsdf, const light& light) const;
};

#endif
