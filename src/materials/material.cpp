#include "material.hpp"
#include "brdf.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"

material::material(brdf* br) : b(br)
{}

const rgbColor material::sample(const point3& p, const vec3& wi, const vec3& wo) const {
	return b->f(wi, wo);
}
