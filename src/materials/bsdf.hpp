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
        bsdf() : diffTra(NULL), diffRef(NULL),
        glossTra(NULL), glossRef(NULL),
        specTra(NULL), specRef(NULL) {}
        ~bsdf();

        virtual const rgbColor f(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;

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
        bxdf* diffTra, *diffRef,
             *glossTra, *glossRef;

        specularBxdf* specTra, *specRef;
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

        virtual const rgbColor evalFresnel(const float& cosThetaO) const {
            return rescaledApproxFresnel(ior, k, cosThetaO);
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
        rgbColor ks;
        const float n;
};

class microfacetDistribution {
    public:
        microfacetDistribution(const rgbColor& r) : rho(r) {}

        virtual ~microfacetDistribution() {}

        virtual const float D(const vec3& wh) const = 0;

        // Default is the TS shadowing masking function.
        inline virtual const float G(const vec3& wo, const vec3& wi, const vec3& wh) const {
            const float ndotwo = fabs(bsdf::cosTheta(wo));
            const float ndotwi = fabs(bsdf::cosTheta(wi));
            const float ndotwh = fabs(bsdf::cosTheta(wh));
            const float wodotwh = fabs(dot(wo, wh));
            return min(1.f, min(2.f * ndotwh * ndotwo / wodotwh,
                        2.f * ndotwh * ndotwi / wodotwh));
        }

        virtual const float pdf(const vec3& wo, const vec3& wi) const = 0;
        virtual void sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const = 0;

        const rgbColor rho;
};

/**
 * Torrance/Sparrow microfacet BRDF model.
 */
class microfacetBrdf : public bxdf {
    public:
        microfacetBrdf(const float& e, const float& K,
                microfacetDistribution* d) : bxdf(bxdfType(GLOSSY | REFLECTION)), eta(e), k(K), distrib(d) {}

        virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            const vec3 wh = halfVector(wo, wi);
            const float cosThetaO = bsdf::cosTheta(wo);
            const float cosThetaI = bsdf::cosTheta(wi);
            const float cosThetaH = dot(wi, wh);
            const rgbColor F = rescaledApproxFresnel(eta, k, cosThetaH);

            return 
                distrib->rho * distrib->D(wh) * distrib->G(wo, wi, wh) * F /
                (4.f * cosThetaO * cosThetaI);
        }

        inline virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const{
            distrib->sampleF(u0, u1, wo, wi, pd);
            return f(wo, wi);
        }

        inline virtual const float pdf(const vec3& wo, const vec3& wi) const{
            return distrib->pdf(wo, wi);
        }

        ~microfacetBrdf(){
            delete distrib;
        }

    private:
        const float eta, k;
        const microfacetDistribution* distrib;
};

/**
 * Cornell's Microfacet BTDF model.
 */
/*
class microfacetBtdf : public bxdf {
    public:
        microfacetBtdf(const float& e, const float& K,
                microfacetDistribution* d) :
            bxdf(bxdfType(GLOSSY | TRANSMISSION)), eta(e), k(K), distrib(d) {} 

        virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            const bool entering = (cosThetaO > 0.f);

            const float eta1 = entering ? 1.0029f : eta;
            const float eta2 = entering ? eta : 1.0029f;

            const vec3 wh = halfVector(eta1 * wo, eta2 * wi);

            const float cosThetaO = bsdf::cosTheta(wo);
            const float cosThetaI = bsdf::cosTheta(wi);
            const float cosThetaH = dot(wi, wh);

            const rgbColor Ft = rgbColor(1.f) - rescaledApproxFresnel(eta, k, cosThetaH);
        }

    private:
};
*/

/**
 * Bilnn microfacet distribution.
 */
class blinn : public microfacetDistribution {
    public:
        blinn(const rgbColor& r, const float& e) :
            microfacetDistribution(r), exp(e)
        {}

        inline virtual const float D(const vec3& wh) const {
            return (exp+2.f) * INVTWOPI * powf(bsdf::cosTheta(wh), exp);
        }

        virtual void sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        float exp;
};

/**
 * Ashikhmin-Shirley anisotropic microfacet distribution.
 */
class aniso : public microfacetDistribution {
    public:
        aniso(const rgbColor& r, const float& Nu, const float& Nv) :
            microfacetDistribution(r), nu(Nu), nv(Nv), ecTerm(sqrt((Nu+2.f)*(Nv+2.f)) * INVTWOPI)
        {}

        virtual const float D(const vec3& wh) const {
            return ecTerm * powf(fabs(bsdf::cosTheta(wh)), exponent(wh));
        }

        virtual void sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        inline const float exponent(const vec3& wh) const{
            return ((nu * wh.x() * wh.x()) + (nv * wh.z() * wh.z())) / (1.f - wh.y() * wh.y());
        }

        float nu, nv;

        // Energy conservation term.
        float ecTerm;
};

/**
 * Beckmann Distribution
 */
class beckmann : public microfacetDistribution {
    public:
        beckmann(const rgbColor& r, const float& a) : microfacetDistribution(r), alpha(a) {}

        virtual const float D(const vec3& wh) const {
            const float cosThetaH = bsdf::cosTheta(wh);
            if(cosThetaH > 0.f){
                const float thetaH = acosf(cosThetaH);
                const float tanThetaH = tanf(thetaH);
                const float e = -(tanThetaH * tanThetaH) / (alpha * alpha);
                return (exp(e) * INVPI) / (alpha * alpha * pow(cosThetaH, 4));
            }else{
                return 0.f;
            }
        }

        inline virtual const float G(const vec3& wo, const vec3& wi, const vec3& wh) const {
            return G1(wo, wh) * G1(wi, wh);
        }

        virtual void sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        inline const float G1(const vec3& v, const vec3& wh) const{
            if(dot(v,wh) / bsdf::cosTheta(v) > 0.f){
                const float cosThetaH = bsdf::cosTheta(wh);
                const float a = 1.f / (alpha * tanf(acosf(cosThetaH)));
                if(a < 1.6f){
                    return (3.535f * a + 2.181* a*a) / (1.f + 2.276*a + 2.577 * a*a);
                }else{
                    return 1.f;
                }
            }else{
                return 0.f;
            }
        }

        float alpha;
};

class newWard : public bxdf {
	public:
		newWard(const rgbColor& rs, const float& a, const float& b) :
			bxdf(bxdfType(GLOSSY | REFLECTION)), Rs(rs), alpha(a), beta(b) {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const;

        virtual const float pdf(const vec3& wo, const vec3& wi) const;

	private:
		rgbColor Rs;
		float alpha, beta;
};

class substrate : public bxdf {
    public:
        substrate(const rgbColor& rd, const rgbColor& rs, microfacetDistribution* d) : bxdf(bxdfType(GLOSSY | REFLECTION)),
            Rd(rd), Rs(rs), ecTerm(((28.f * rd) / (23.f * PI)) * rs.inverse()),
            distrib(d)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const;

        virtual const float pdf(const vec3& wo, const vec3& wi) const;

        ~substrate(){
            delete distrib;
        }

    private:
        rgbColor Rd, Rs;
        rgbColor ecTerm;
        const microfacetDistribution* distrib;
};

typedef shared_ptr<bsdf> bsdfPtr;
typedef shared_ptr<bxdf> bxdfPtr;
