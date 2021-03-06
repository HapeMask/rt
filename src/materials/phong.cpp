#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

rgbColor phongBrdf::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    const float sinAlpha = sqrt(1.f - pow(u0, 2.f / (float)(n+1)));
    const float phi = TWOPI * u1;

    wi.x = sinAlpha * cos(phi);
    wi.y = pow(u0, (1.f / (float)(n+1)));
    wi.z = sinAlpha * sin(phi);

    if(wi.y < 0){
        pd = 0.f;
        return rgbColor(0.f);
    }else{
        pd = pdf(wo, wi);
        return f(wo, wi);
    }
}

rgbColor phongBrdf::f(const vec3& wo, const vec3& wi) const{
    const float cosAlpha = dot(wo, reflect(wi));
    if(cosAlpha < 0){
        return rgbColor(0.f);
    }else{
        return ks * (float)(n+2) * INVTWOPI * pow(cosAlpha, n);
    }
}

float phongBrdf::pdf(const vec3& wo, const vec3& wi) const {
    const float cosAlpha = dot(wo, reflect(wi));
    if(cosAlpha < 0){
        return 0.f;
    }else{
        return ((float)(n+1) * INVTWOPI) * pow(cosAlpha, n);
    }
}
