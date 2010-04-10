#ifndef __RT_MATERIAL_
#define __RT_MATERIAL_

#include <tr1/memory>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "brdf.hpp"

using namespace std;
using tr1::shared_ptr;

class material {
public:
	material(brdfPtr br);
	material(const rgbColor& c, const float& pow);

	const rgbColor sample(const point3& p, const vec3& wi, const vec3& wo) const;
	const rgbColor sampleL() const;

	const bool& isEmissive(){
		return emissive;
	}

protected:
	brdfPtr b;
	bool emissive;
	rgbColor emitColor;
	float emitPower;
};

typedef shared_ptr<material> materialPtr;
#endif
