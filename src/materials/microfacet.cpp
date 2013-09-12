#include "bsdf.hpp"
#include "samplers/samplers.hpp"

microfacetBrdf::microfacetBrdf(const float& e, const float& K, microfacetDistribution* d) :
    bxdf(bxdfType(GLOSSY | REFLECTION)), eta(e), k(K), distrib(d) {}

microfacetBrdf::~microfacetBrdf(){
    delete distrib;
}

float microfacetDistribution::G(const vec3& wo, const vec3& wi, const vec3& wh) const {
    const float ndotwo = abs(bsdf::cosTheta(wo));
    const float ndotwi = abs(bsdf::cosTheta(wi));
    const float ndotwh = abs(bsdf::cosTheta(wh));
    const float wodotwh = abs(dot(wo, wh));
    return std::min(1.f, std::min(2.f * ndotwh * ndotwo / wodotwh,
                2.f * ndotwh * ndotwi / wodotwh));
}

rgbColor microfacetBrdf::f(const vec3& wo, const vec3& wi) const {
    const vec3 wh = halfVector(wo, wi);
    const float cosThetaO = abs(bsdf::cosTheta(wo));
    const float cosThetaT = abs(bsdf::cosTheta(wi));
    const float cosThetaH = dot(wi, wh);
    const rgbColor F = evalFresnel(cosThetaH);

    return 
        F * distrib->rho * distrib->D(wh) * distrib->G(wo, wi, wh) /
        (4.f * cosThetaO * cosThetaT);
}

rgbColor microfacetBrdf::evalFresnel(const float& cosTheta) const {
    return rescaledApproxFresnel(eta, k, cosTheta);
}

rgbColor microfacetBrdf::sampleF(const float& u0, const float& u1,
        const vec3& wo, vec3& wi, float& pd) const {
    distrib->sampleF(u0, u1, wo, wi, pd);
    return f(wo, wi);
}

float microfacetBrdf::pdf(const vec3& wo, const vec3& wi) const {
    return distrib->pdf(wo, wi);
}

microfacetBtdf::microfacetBtdf(const float& e, const float& K, microfacetDistribution* d) :
    bxdf(bxdfType(GLOSSY | TRANSMISSION)), eta(e), k(K), distrib(d)
{}

microfacetBtdf::~microfacetBtdf(){
    delete distrib;
}

rgbColor microfacetBtdf::f(const vec3& wo, const vec3& wi) const {
    return rgbColor(0.f);
}

rgbColor microfacetBtdf::evalFresnel(const float& cosTheta) const {
    return rescaledApproxFresnel(eta, k, cosTheta);
}

rgbColor microfacetBtdf::sampleF(const float& u0, const float& u1,
        const vec3& wo, vec3& wi, float& pd) const
{
    distrib->sampleF(u0, u1, wo, wi, pd);

    const bool entering = (bsdf::cosTheta(wo) > 0.f);

    const float etaO = entering ? 1.0029f : eta;
    const float etaT = entering ? eta : 1.0029f;

    const float etaR = etaO / etaT;
    const float eta2 = etaR*etaR;

    const vec3 m = halfVector(wo, wi);
    const float cosThetaO = dot(wo, m);
    const float sin2ThetaT = eta2 * (1.f - cosThetaO*cosThetaO);
    // Total Internal Reflection
    if(sin2ThetaT > 1.f){
        pd = 0.f;
        return rgbColor(0.f);
        wi = 2.f * abs(dot(wo, m)) * m - wo;
        pd = distrib->pdf(wo, wi);
        return rgbColor(255,255,255);
    }

    // Flip the normal if we're entering the surface.
    const float cosThetaT = entering ?
        -sqrt(1.f - sin2ThetaT) :
        sqrt(1.f - sin2ThetaT);

    wi = normalize(etaR * -wo + (etaR * cosThetaO + cosThetaT) * m);

    /*
    const vec3 whT = -halfVector(etaO * wo, etaT * wi);
    const float jacobian = (etaT*etaT * abs(dot(wi, m))) / pow(etaO * dot(wo, m) + etaT * dot(wo, m), 2);
    pd *= jacobian;
    */

    pd = 1.f;

    const rgbColor Fr = rescaledApproxFresnel(eta, k, abs(bsdf::cosTheta(wo)));
    return eta2 * (rgbColor(1.f) - Fr) * distrib->rho / abs(bsdf::cosTheta(wi));
}

float microfacetBtdf::pdf(const vec3& wo, const vec3& wi) const {
    return 0.f;
}
