#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor phongBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float sinAlpha = sqrtf(1.f - powf(u0, 2.f / (float)(n+1)));
    const float phi = TWOPI * u1;

    wi.x() = sinAlpha * cosf(phi);
    wi.y() = powf(u1, (1.f / (float)(n+1)));
    wi.z() = sinAlpha * sinf(phi);

    // Treat N<1000 as glossy, N>=1000 as specular.
    if(n < 1000){
        pd = pdf(wo, wi);
        return f(wo, wi);
    }else{
        pd = 1.f;
        return 1.f;
    }
}

const rgbColor phongBrdf::f(const vec3& wo, const vec3& wi) const{
    // -wo.z = cos(perfect specular reflection dir)
    return ks * (float)(n+2) * INVTWOPI *
        powf(fabs(dot(wo, vec3(-wi.x(), wi.y(), -wi.z()))), n);
}

const float phongBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return ((float)(n+1) * INVTWOPI) * powf(fabs(dot(wo, vec3(-wi.x(), wi.y(), -wi.z()))), n);
}
