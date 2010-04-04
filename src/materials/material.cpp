#include "material.hpp"
#include "brdf.hpp"

material::material(const brdf* br) : b(br)
{}

const rgbColor sample(const point& p, const vec3& wi, const vec3& wo) const {
	return b->f(wi, wo);
}
