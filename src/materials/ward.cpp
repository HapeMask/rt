#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor newWard::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
	const float phi = atan2(beta * sinf(TWOPI * u1), alpha * cosf(TWOPI * u1));
	const float cosPhi = cosf(phi);
	const float sinPhi = sinf(phi);
	const float theta = atanf(sqrt(-logf(1.f - u0) /
				((cosPhi*cosPhi)/(alpha*alpha) + (sinPhi*sinPhi)/(beta*beta))));

    vec3 wh;
    sphericalToDirection(wh, sinf(theta), cosf(theta), phi);
    wh.normalize();

	// Find the incident direction that corresponds to this half vector and the
	// reflected direction.
    wi = -wo + 2.f * fabs(dot(wo, wh)) * wh;

	const rgbColor F = f(wo, wi);

	if(wo.y() * wi.y() < 0.f){
		pd = 0.f;
	}else{
		const float y = F.gray();
		const float ps = Rs.gray();
		pd = y /
			((2.f * ps) / (1.f + wo.y()*wi.y()));
	}

	return F;
}

const rgbColor newWard::f(const vec3& wo, const vec3& wi) const{
	const vec3 wh = halfVector(wo, wi);
	const float cos2Phi = wh.x() * wh.x();
	const float sin2Phi = wh.z() * wh.z();
	//const float cosThetaO = (bsdf::cosTheta(wo));
	//const float cosThetaI = (bsdf::cosTheta(wi));

	//const float sinThetaO = (bsdf::sinTheta(wo));
	//const float sinThetaI = (bsdf::sinTheta(wi));

	return (Rs * INVPI / (alpha*beta)) * 
		exp(
				(-1.f / (wh.y()*wh.y())) *
				((cos2Phi / (alpha*alpha)) +
				 (sin2Phi / (beta*beta)))
		) * ((dot(wh,wh)) / pow(wh.y(), 4));
}

const float newWard::pdf(const vec3& wo, const vec3& wi) const{
	if(wo.y()*wi.y() < 0.f){
		return 0.f;
	}

	const rgbColor fc = f(wo, wi);
	const float y = fc.gray();
	const float ps = Rs.gray();

	return y /
		((2.f * ps) / (1.f + wo.y()*wi.y()));
}
