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

    wi.x = sinAlpha * cosf(phi);
    wi.y = powf(u0, (1.f / (float)(n+1)));
    wi.z = sinAlpha * sinf(phi);

    if(wi.y < 0){
        pd = 0.f;
        return 0.f;
    }else{
        pd = pdf(wo, wi);
        return f(wo, wi);
    }
}

const rgbColor phongBrdf::f(const vec3& wo, const vec3& wi) const{
    const float cosAlpha = dot(wo, reflect(wi));
    if(cosAlpha < 0){
        return 0.f;
    }else{
        return ks * (float)(n+2) * INVTWOPI * powf(cosAlpha, n);
    }
}

const float phongBrdf::pdf(const vec3& wo, const vec3& wi) const {
    const float cosAlpha = dot(wo, reflect(wi));
    if(cosAlpha < 0){
        return 0.f;
    }else{
        return ((float)(n+1) * INVTWOPI) * powf(cosAlpha, n);
    }
}
