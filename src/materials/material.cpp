#include "material.hpp"
#include "brdf.hpp"

const rgbColor sample(const point& p, const vec3& wi, const vec3& wo) const {
	return b.f(wi, wo);
}
