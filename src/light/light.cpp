#include "light.hpp"
#include "mathlib/point.hpp"
#include "color/color.hpp"

pointLight::pointLight(const point3& p, const rgbColor& c, const float& pow) : light(p, pow), lightColor(c)
{}

const rgbColor pointLight::L(const point3& p) const {
	return lightColor;
}
