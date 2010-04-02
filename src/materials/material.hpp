#ifndef __RT_MATERIAL_
#define __RT_MATERIAL_

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "brdf.hpp"

class material {
	public:
		material(const brdf& br);
		const rgbColor sample(const point& p, const vec3& wi, const vec3& wo) const;

	protected:
		brdf b;
};
#endif
