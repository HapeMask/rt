#include "bsdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"

#include <cmath>

const rgbColor asPhong::sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
    // Decide whether to sample the diffuse or specular components, then rescale u0.
    if(u0 < 0.5f){
        const float u0new = 2.f * u0;

        cosineSampleHemisphere(wi, u0new, u1);
        pd = pdf(wo, wi);
        return f(wo, wi);
    }else{
        const float u0new = 2.f * (u0 - 0.5f);

        // Generate a vector wh with distribution described by pdf().
        float phi, costheta;
        if (u0 < .25f) {
            sampleFirstQuadrant(4.f * u0new, u1, nu, nv, phi, costheta);
        } else if (u0new < .5f) {
            const float u = 4.f * (.5f - u0new);
            sampleFirstQuadrant(u, u1, nu, nv, phi, costheta);
            phi = M_PI - phi;
        } else if (u0new < .75f) {
            const float u = 4.f * (u0new - .5f);
            sampleFirstQuadrant(u, u1, nu, nv, phi, costheta);
            phi += M_PI;
        } else {
            const float u = 4.f * (1.f - u0new);
            sampleFirstQuadrant(u, u1, nu, nv, phi, costheta);
            phi = 2.f * M_PI - phi;
        }

        const float sintheta = sqrtf(max(0.f, 1.f - costheta*costheta));
        vec3 wh = normalize(vec3(sintheta*costheta, phi, sintheta*sintheta));

        wi = -wo + 2.f * dot(wo, wh) * wh;
        pd = pdf(wo, wi);

        if(wo.y() * wi.y() < 0.f){
            return 0.f;
        }else{
            // Compute incident direction by reflecting about wh.
            return f(wo, wi);
        }
    }
}

const float asPhong::pdf(const vec3& wo, const vec3& wi) const{
    // Discard if in same hemisphere.
    if(wo.y() * wi.y() < 0){
        return 0.f;
    }else{
        const vec3 wh = halfVector(wo, wi);

        // Return the average of the diffuse and specular component PDFs.
        return 0.5f * (
                bsdf::cosTheta(wi) * INVPI +
                (pdfTerm * pow(bsdf::cosTheta(wh), exponent(wh)) / (4.f * dot(wo, wh)))
                );
    }
}

const rgbColor asPhong::f(const vec3& wo, const vec3& wi) const{
    const vec3 wh = halfVector(wo, wi);
    return rhoS(wo, wi, wh) + rhoD(wo, wi);
}

const rgbColor asPhong::rhoS(const vec3& wo, const vec3& wi, const vec3& wh) const{
    const float wiDotWh = dot(wi, wh);

    return Rs * ecTermS * powf(bsdf::cosTheta(wh), exponent(wh)) * evalFresnel(wiDotWh) /
        (wiDotWh * max(bsdf::cosTheta(wo), bsdf::cosTheta(wi)));
}

const rgbColor asPhong::rhoD(const vec3& wo, const vec3& wi) const{
    return ecTermD *
        (1.f - powf(1.f - (bsdf::cosTheta(wo) / 2.f), 5.f)) *
        (1.f - powf(1.f - (bsdf::cosTheta(wi) / 2.f), 5.f));
}
