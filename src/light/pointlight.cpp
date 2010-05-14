#include "light.hpp"
#include "mathlib/point.hpp"
#include "color/color.hpp"
#include "mathlib/vector.hpp"

pointLight::pointLight(const point3& p, const float& pow, const rgbColor& c) : light(p, pow, c)
{}

const rgbColor pointLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const {
    pdf = 1.f;
    wi = position - p;
    return L(p);
}