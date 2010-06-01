#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

/*
const rgbColor anisoPhongBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
	// Sample from first quadrant and remap to hemisphere to sample \wh
	float phi, costheta;
	if (u0 < .25f) {
		sampleFirstQuadrant(4.f * u0, u1, phi, costheta);
	} else if (u0 < .5f) {
		const float u = 4.f * (.5f - u0);
		sampleFirstQuadrant(u0, u1, phi, costheta);
		phi = M_PI - phi;
	} else if (u0 < .75f) {
		const float u = 4.f * (u0 - .5f);
		sampleFirstQuadrant(u0, u1, phi, costheta);
		phi += M_PI;
	} else {
		const float u = 4.f * (1.f - u0);
		sampleFirstQuadrant(u0, u1, phi, costheta);
		phi = 2.f * M_PI - phi;
	}

	const float sintheta = sqrtf(max(0.f, 1.f - costheta*costheta));
	vec3 wh = vec3(sintheta*costheta, phi, sintheta*sintheta);
	if (dot(wo, wh) < 0.f) wh = -wh;

	// Compute incident direction by reflecting about $\wh$
    wi = -wo + 2.f * dot(wo, wh) * wh;

	// Compute PDF from Anisotropic distribution
	pd = (dot(wo, wh) < 0.f) ? 0.f : microfacetDistrib(wh) / (4.f * dot(wo, wh));
	
    return f(wo, wi);
}

const float anisoPhongBrdf::pdf(const vec3& wo, const vec3& wi) const{
}
*/
