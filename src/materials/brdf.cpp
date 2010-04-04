#include "brdf.hpp"
#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"

lambertianBrdf::lambertianBrdf(const rgbColor& r) : rOverPi(r / PI)
{}

const rgbColor lambertianBrdf::f(const vec3& wi, const vec3& wo) const {
	return rOverPi;
}
