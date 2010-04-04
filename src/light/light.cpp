#include "light.hpp"
#include "mathlib/point.hpp"
#include "color/color.hpp"

pointLight::pointLight(const point3& p) : light(p), _color(1.f, 1.f, 1.f)
{}

pointLight::pointLight(const point3& p, const rgbColor& c) : light(p), _color(c)
{}

const rgbColor pointLight::L(const vec3& wo) const {
	return _color;
}
