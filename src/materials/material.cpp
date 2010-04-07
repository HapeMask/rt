#include "material.hpp"
#include "brdf.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"

material::material(brdf* br) : b(br), emissive(false)
{}

material::material(const rgbColor& c, const float& pow) : emissive(true),
		emitColor(c), emitPower(pow)
{}

const rgbColor material::sample(const point3& p, const vec3& wi, const vec3& wo) const {
	if(!emissive){
		return b->f(wi, wo);
	}else{
		return sampleL();
	}
}

const rgbColor material::sampleL() const {
	return emitColor * emitPower;
}