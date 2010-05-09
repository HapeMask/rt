#include "material.hpp"
#include "bsdf.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"

material::material(bsdfPtr br) : b(br), emissive(false)
{}

material::material(const rgbColor& c, const float& pow) : emissive(true),
		emitColor(c), emitPower(pow)
{}

const rgbColor material::sample(const point3& p, const vec3& wi, const vec3& wo, bxdfType type) const {
	if(!emissive){
		return b->f(wi, wo, type);
	}else{
		return Le();
	}
}

const rgbColor material::Le() const {
	return emitColor * emitPower;
}
