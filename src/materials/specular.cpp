#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor specularBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    pd = 1.f;
    wi = vec3(-wo.x(), wo.y(), -wo.z());

    const rgbColor Fr = evalFresnel(fabs(bsdf::cosTheta(wo)));
    return Fr * kR / fabs(bsdf::cosTheta(wi));
}

const rgbColor specularBtdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    pd = 1.f;
    const bool entering = (bsdf::cosTheta(wo) > 0.f);

    // The normal always points out of the object (as in PBRT), thus
    // we need to check if the ray is inside the object by checking
    // the sign of the angle between the ray and the normal.
    //
    // Swaps IOR accordingly and generates a refraction normal
    // that always points in the same direction as the ray.
    const float eta = entering ? (1.f / ior) : ior;
    const float eta2 = eta*eta;

    const float sin2ThetaT = eta2 * bsdf::sin2Theta(wo);
    // Total Internal Reflection
    if(sin2ThetaT > 1.f){
        return 0.f;
    }

    // Flip the normal if we're entering the surface.
    const float cosThetaT = entering ?
        -sqrtf(max(0.f, 1.f - sin2ThetaT)) :
        sqrtf(max(0.f, 1.f - sin2ThetaT));

    wi.x() = eta * -wo.x();
    wi.y() = cosThetaT;
    wi.z() = eta * -wo.z();

    // abs(cosTheta) "flips the normal" for us.
    const rgbColor Fr = evalFresnel(fabs(bsdf::cosTheta(wo)));
    return (1.f/eta2) * (rgbColor(1.f) - Fr) * kT / fabs(bsdf::cosTheta(wi));
}
