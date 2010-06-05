#pragma once

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include <tr1/memory>
using std::tr1::shared_ptr;

enum bxdfType {
    REFLECTION =        1<<0,
    TRANSMISSION =      1<<1,
    DIFFUSE =           1<<2,
    GLOSSY =            1<<3,
    SPECULAR =          1<<4,
    ALL_REFLECTION =    DIFFUSE | GLOSSY | SPECULAR | REFLECTION,
    ALL_TRANSMISSION =  DIFFUSE | GLOSSY | SPECULAR | TRANSMISSION,
    ALL =               ALL_REFLECTION | ALL_TRANSMISSION
};

enum fresnelType {
    CONDUCTOR = 0,
    DIELECTRIC = 1
};

//
// Utility Functions
//

inline const rgbColor rescaledApproxFresnel(const float& eta, const float& k, const float& cosTheta){
    return ((eta-1.f)*(eta-1.f) + (4 * eta * powf(1.f - cosTheta, 5) + k*k)) /
        ((eta+1.f)*(eta+1.f) + k*k);
}

inline const rgbColor schlickFresnel(const rgbColor& r0, const float& cosTheta){
	return r0 + (rgbColor(1.f) - r0) * powf(1.f - cosTheta, 5);
}

inline const vec3 halfVector(const vec3& wo, const vec3& wi){
    return normalize(wo + wi);
}

inline const vec3 reflect(const vec3& wo){
    return vec3(-wo.x(), wo.y(), -wo.z());
}

class bxdf;
class specularBxdf;
class bsdf {
    public:
        bsdf() : specTra(NULL), specRef(NULL),
        diffTra(NULL), diffRef(NULL),
        glossTra(NULL), glossRef(NULL) {}
        ~bsdf();

        virtual const rgbColor f(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;
        const rgbColor sampleF(const float& u0, const float& u1, const float& u2,
                const vec3& wo, vec3& wi, bxdfType type, bxdfType& sampledType) const;

        const rgbColor sampleF(const float& u0, const float& u1, const float& u2,
                const vec3& wo, vec3& wi, bxdfType type, bxdfType& sampledType, float& pd) const;

        void addBxdf(bxdf* b);

        const float pdf(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;

        inline static const bool isSupertype(bxdfType a, bxdfType b) {
            return (a & b);
        }

        inline static const float cosTheta(const vec3& v){
            return v.y();
        }

        inline static const float cos2Theta(const vec3& v){
            return v.y()*v.y();
        }

        inline static const float sinTheta(const vec3& v){
            // sintheta(v) = sqrt(1 - cos2theta(v))
            return sqrtf(max(0.f, 1.f - v.y()*v.y()));
        }

        inline static const float sin2Theta(const vec3& v){
            const float c = cosTheta(v);
            return 1.f - c*c;
        }

    private:
        bxdf *diffTra, *diffRef,
             *glossTra, *glossRef;

        specularBxdf *specRef, *specTra;
};

class bxdf {
    public:
        bxdf(const bxdfType t) : type(t) {}

        virtual const rgbColor f(const vec3& wo, const vec3& wi) const = 0;
        virtual const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi, float& pd) const;
        virtual ~bxdf() {}

        const bxdfType getType() const {
            return type;
        }

        inline const bool isType(const bxdfType t) const {
            return (type & t) == type;
        }

        inline virtual const float pdf(const vec3& wo, const vec3& wi) const {
            // PDF for solid angle over entire hemisphere:
            // Cos(Theta)/Pi
            return bsdf::cosTheta(wi) * INVPI;
        }

    private:
        const bxdfType type;
};

class specularBxdf : public bxdf {
    public:
        specularBxdf(const bxdfType type, const float& eta, const float& K, const fresnelType ft) :
            bxdf(type), fType(ft), ior(eta), k(K) {}

        virtual const rgbColor evalFresnel(const float& cosThetaI) const {
            return rescaledApproxFresnel(ior, k, cosThetaI);
        }

    protected:
        const fresnelType fType;
        const float ior;
        const float k;
};

class lambertianBrdf : public bxdf {
    public:
        lambertianBrdf(const rgbColor& r) : bxdf(bxdfType(DIFFUSE | REFLECTION)), rOverPi(r * INVPI) {}

        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const{
            return rOverPi;
        }

    private:
        const rgbColor rOverPi;
};

class specularBrdf : public specularBxdf {
    public:
        specularBrdf(const float& i, const float& K, const fresnelType ft, rgbColor kr) :
            specularBxdf(bxdfType(SPECULAR | REFLECTION), i, K, ft), kR(kr)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            return 0.f;
        }

        inline virtual const float pdf(const vec3& wo, const vec3& wi) const{
            return 0.f;
        }

    private:
        const rgbColor kR;
};

class specularBtdf : public specularBxdf {
    public:
        specularBtdf(const float& i, const rgbColor& kt) :
            specularBxdf(bxdfType(SPECULAR | TRANSMISSION), i, 0.f, DIELECTRIC), kT(kt)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            return 0.f;
        }

        inline virtual const float pdf(const vec3& wo, const vec3& wi) const {
            return 0.f;
        }

    private:
        const rgbColor kT;
};

class phongBrdf : public bxdf {
    public:
        phongBrdf(const rgbColor& s, const float& N) : bxdf(bxdfType(GLOSSY | REFLECTION)), ks(s), n(N)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const; 

        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        const rgbColor ks;
        const float n;
};

/**
 * Torrance/Sparrow microfacet BRDF model.
 */
class microfacetBxdf : public bxdf {
    public:
        microfacetBxdf(const rgbColor r, const float& e, const float& K, const bxdfType type) : bxdf(type), Rs(r), eta(e), k(K) {}

        virtual const float microfacetDistrib(const vec3& wh) const = 0;

        const float attenuation(const vec3& wo, const vec3& wi, const vec3& wh) const {
            const float ndotwo = fabs(bsdf::cosTheta(wo));
            const float ndotwi = fabs(bsdf::cosTheta(wi));
            const float ndotwh = fabs(bsdf::cosTheta(wh));
            const float wodotwh = fabs(dot(wo, wh));
            return min(1.f, min(2.f * ndotwh * ndotwo / wodotwh,
                        2.f * ndotwh * ndotwi / wodotwh));
        }

        //virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const = 0;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            const vec3 wh = halfVector(wo, wi);
            const float cosThetaO = bsdf::cosTheta(wo);
            const float cosThetaI = bsdf::cosTheta(wi);
            const rgbColor Fr = rescaledApproxFresnel(eta, k, cosThetaO);

            return
                (Rs * microfacetDistrib(wh) * attenuation(wo, wi, wh) * Fr) /
                (4.f * cosThetaO * cosThetaI);
        }

        virtual const float pdf(const vec3& wo, const vec3& wi) const = 0;

        inline const rgbColor& rs() const {
            return Rs;
        }

    private:
        const rgbColor Rs;
        const float eta, k;
};

/**
 * Bilnn microfacet distribution.
 */
class blinnMicrofacet : public microfacetBxdf {
    public:
        blinnMicrofacet(const rgbColor& r, const float& eta, const float& k, const float& e) :
            microfacetBxdf(r, eta, k, bxdfType(GLOSSY | REFLECTION)), exp(e)
        {}

        virtual const float microfacetDistrib(const vec3& wh) const {
            return (exp+2.f) * INVTWOPI * powf(bsdf::cosTheta(wh), exp);
        }

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        const float exp;
};

//, pdfTerm(sqrt((nu+1.f)*(nv+1.f)) * INVTWOPI) 

/**
 * Ashikhmin-Shirley anisotropic microfacet distribution.
 */
class asMicrofacet : public microfacetBxdf {
    public:
        asMicrofacet(const rgbColor& r, const float& eta, const float& k, const float& Nu, const float& Nv) :
            microfacetBxdf(r, eta, k, bxdfType(GLOSSY | REFLECTION)), nu(Nu), nv(Nv), ecTerm(sqrt((Nu+2.f)*(Nv+2.f)) * INVTWOPI)
        {}

        virtual const float microfacetDistrib(const vec3& wh) const {
            return ecTerm * powf(bsdf::cosTheta(wh), exponent(wh));
        }

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        inline const float exponent(const vec3& wh) const{
            return ((nu * wh.x() * wh.x()) + (nv * wh.z() * wh.z())) / (1.f - wh.y() * wh.y());
        }

        const float nu, nv;

        // Energy conservation terms.
        const float ecTerm;
};

class substrate : public bxdf {
    public:
        substrate(const rgbColor& rd, const rgbColor& rs, microfacetBxdf* rhos) : bxdf(bxdfType(GLOSSY | REFLECTION)), Rd(rd), Rs(rs), rhoS(rhos),
        ecTerm(((28.f * rd) / (23.f * PI)) * rs.inverse())
        {}

        ~substrate(){
            delete rhoS;
        }

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const;

        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        const rgbColor Rd, Rs;
        const rgbColor ecTerm;
        microfacetBxdf* rhoS;
};

typedef shared_ptr<bsdf> bsdfPtr;
typedef shared_ptr<bxdf> bxdfPtr;
