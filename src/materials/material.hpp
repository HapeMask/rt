#ifndef __RT_MATERIAL_
#define __RT_MATERIAL_

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "brdf.hpp"

class material {
	public:
		material(const brdf& br);
		virtual const rgbColor sample(const point& p, const vec3& wi, const vec3& wo) = 0;

	protected:
		brdf b;
};

class lambertMaterial : public material {
	public:
		lambertMaterial(const brdf& br) : material(br) {}
		virtual const rgbColor sample(const point& p, const vec3& wi, const vec3& wo) const;
};
#endif
