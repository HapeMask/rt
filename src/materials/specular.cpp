#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"

#include <cmath>

const rgbColor specularBxdf::evalFresnel(const float& cosThetaO) const {
    return rescaledApproxFresnel(ior, k, cosThetaO);
}

const rgbColor specularBrdf::f(const vec3& wo, const vec3& wi) const {
    return rgbColor(0.f);
}

float specularBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return 0.f;
}

const rgbColor specularBtdf::f(const vec3& wo, const vec3& wi) const {
    return rgbColor(0.f);
}

float specularBtdf::pdf(const vec3& wo, const vec3& wi) const {
    return 0.f;
}

const rgbColor specularBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    pd = 1.f;
    wi = normalize(vec3(-wo.x, wo.y, -wo.z));

    const rgbColor Fr = evalFresnel(abs(bsdf::cosTheta(wo)));
    return Fr * kR / abs(bsdf::cosTheta(wi));
}

const rgbColor specularBtdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    pd = 1.f;
    // The normal always points out of the object (as in PBRT), thus
    // we need to check if the ray is inside the object by checking
    // the sign of the angle between the ray and the normal.
    //
    // Swaps IOR accordingly and generates a refraction normal
    // that always points in the same direction as the ray.
    const bool entering = (bsdf::cosTheta(wo) > 0.f);

    const float eta = entering ? (1.0029f / ior) : (ior / 1.0029f);
    const float eta2 = eta*eta;

    const float sin2ThetaT = eta2 * bsdf::sin2Theta(wo);
    // Total Internal Reflection
    if(sin2ThetaT > 1.f){
        if(entering) {
            wi = normalize(vec3(-wo.x, wo.y, -wo.z));
        }else{
            wi = normalize(vec3(-wo.x, -wo.y, -wo.z));
        }

        const rgbColor Fr = evalFresnel(abs(bsdf::cosTheta(wo)));
        return Fr / abs(bsdf::cosTheta(wi));
    }

    // Flip the normal if we're entering the surface.
    const float cosThetaT = entering ?
        -sqrt(1.f - sin2ThetaT) :
        sqrt(1.f - sin2ThetaT);

    wi = normalize(vec3(
                eta * -wo.x,
                cosThetaT,
                eta * -wo.z
            ));

    const rgbColor Ft = rgbColor(1.f) - evalFresnel(abs(bsdf::cosTheta(wo)));
    return eta2 * Ft * kT / abs(bsdf::cosTheta(wi));
}
