#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor newWard::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    /*
	//const float phi = atan2(beta * sinf(TWOPI * u1), alpha * cosf(TWOPI * u1));
    const float phi = atanf(beta/alpha * tanf(TWOPI * u1));
	const float cosPhi = cosf(phi);
	const float sinPhi = sinf(phi);
	const float theta = atanf(sqrt(-logf(u0) /
				((cosPhi*cosPhi)/(alpha*alpha) + (sinPhi*sinPhi)/(beta*beta))));

    vec3 wh;
    sphericalToDirection(wh, sinf(theta), cosf(theta), phi);
    wh.normalize();

	// Find the incident direction that corresponds to this half vector and the
	// reflected direction.
    wi = 2.f * fabs(dot(wo, wh)) * wh - wo;
    */
    uniformSampleHemisphere(wi);

    pd = pdf(wo, wi);
    if(pd <= 0){
        cerr << "ERR" << endl;
    }
	return f(wo, wi);
}

const rgbColor newWard::f(const vec3& wo, const vec3& wi) const{
	const vec3 wh = halfVector(wo, wi);

    const rgbColor f = (Rs / (4.f * PI * alpha * beta * sqrtf(wi.y() * wo.y()))) *
        exp( -((wh.x()*wh.x() / (alpha*alpha)) + (wh.z()*wh.z() / (beta*beta))) / (wh.y()*wh.y())
                );
    return f;
    /*
	const float cos2Phi = wh.x() * wh.x();
	const float sin2Phi = wh.z() * wh.z();

	return (Rs * INVPI / (alpha*beta)) * 
		exp(
				(-1.f / (wh.y()*wh.y())) *
				((cos2Phi / (alpha*alpha)) +
				 (sin2Phi / (beta*beta)))
		) * ((dot(wh,wh)) / pow(wh.y(), 4));
        */
}

const float newWard::pdf(const vec3& wo, const vec3& wi) const{
    return bsdf::cosTheta(wi) * INVPI;

	if(wo.y()*wi.y() < 0.f){
		return 0.f;
	}

    const vec3 wh(halfVector(wo, wi));
    const float t = tanf(wh.y() * (wh.x()*wh.x() / (alpha*alpha) + wh.z()*wh.z() / (beta*beta)));
    return (1.f / (4.f * PI * alpha * beta * dot(wo, wh) * powf(wh.y(), 3))) *
        exp(t*t);
    /*
	const rgbColor fc = f(wo, wi);
	const float y = fc.gray();
	const float ps = Rs.gray();

	return y /
		((2.f * ps) / (1.f + wo.y()*wi.y()));
        */
}
