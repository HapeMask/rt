#ifndef __RT_MATERIAL_
#define __RT_MATERIAL_

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "brdf.hpp"

class material {
public:
	material(brdf* br);
	material(const rgbColor& c, const float& pow);

	const rgbColor sample(const point3& p, const vec3& wi, const vec3& wo) const;
	const rgbColor sampleL() const;

	const bool& isEmissive(){
		return emissive;
	}

protected:
	brdf* b;
	bool emissive;
	rgbColor emitColor;
	float emitPower;
};
#endif
