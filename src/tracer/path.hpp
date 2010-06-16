#pragma once

#include "color/color.hpp"
#include "mathlib/ray.hpp"
#include "scene/scene.hpp"

#include "tracer.hpp"

class pathTracer : public rayTracer {
	public:
		pathTracer(scene* p) : rayTracer(p) {}
		virtual const rgbColor L(const ray& r) const;

	private:
		template <bool recursiveSpecular>
		const rgbColor _L(ray& r, const int depth = 0) const;
};
