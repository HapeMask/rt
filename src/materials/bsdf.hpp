#pragma once

#include <memory>
#include <cmath>
using std::shared_ptr;

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"
#include "texture.hpp"

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
inline rgbColor rescaledApproxFresnel(const float& eta, const float& k, const float& cosTheta){
    return rgbColor(((eta-1.f)*(eta-1.f) + (4 * eta * pow(1.f - cosTheta, 5) + k*k)) /
        ((eta+1.f)*(eta+1.f) + k*k));
}

inline rgbColor schlickFresnel(const rgbColor& r0, const float& cosTheta){
	return r0 + (rgbColor(1.f) - r0) * pow(1.f - cosTheta, 5);
}

inline vec3 reflect(const vec3& w) {
    return vec3(-w.x, w.y, -w.z);
}

inline vec3 halfVector(const vec3& a, const vec3& b) {
    return normalize(a+b);
}

class bxdf;
class specularBxdf;

class bsdf {
    public:
        bsdf() {}

        virtual rgbColor f(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;
        virtual rgbColor sampleF(const float& u0, const float& u1, const float&
                u2, const vec3& wo, vec3& wi, bxdfType type, bxdfType& sampledType,
                float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;

        /**
         * NOTE: Takes ownership of bxdf b
         */
        void addBxdf(bxdf* b);

        static bool isSubtype(bxdfType a, bxdfType b);
        static float cosTheta(const vec3& v);
        static float cos2Theta(const vec3& v);
        static float sinTheta(const vec3& v);
        static float sin2Theta(const vec3& v);

        void updateFromUVTexture(const vec2& uv);

    protected:
        unique_ptr<bxdf> diffTra, diffRef, glossTra, glossRef;
        unique_ptr<specularBxdf> specTra, specRef;
};

class testBsdf : public bsdf {
    public:
        testBsdf();
};

class bxdf {
    public:
        bxdf(const bxdfType t) : hasTexture(false), type(t) {}
        virtual ~bxdf() {}

        bxdfType getType() const;
        bool isType(const bxdfType t) const;

        virtual rgbColor f(const vec3& wo, const vec3& wi) const = 0;
        virtual rgbColor sampleF(const float& u1, const float& u2,
                const vec3& wo, vec3& wi, float& pd) const = 0;
        virtual float pdf(const vec3& wo, const vec3& wi) const = 0;

        void setTexture(textureSlot slot, texture2DPtr p);
        const texture2D& getTexture(textureSlot slot) const;
        virtual void updateFromUVTexture(const vec2& uv) = 0;

    protected:
        rgbColor textureLookup(const textureSlot& slot, const vec2& uv) const;

        bool hasTexture;

    private:
        const bxdfType type;
        texture2DPtr textureSlots[NUM_TEXTURE_SLOTS];
};

class specularBxdf : public bxdf {
    public:
        specularBxdf(const bxdfType type, const float& eta, const float& K, const fresnelType ft) :
            bxdf(type), fType(ft), ior(eta), k(K) {}

        virtual rgbColor evalFresnel(const float& cosThetaO) const;
        virtual void updateFromUVTexture(const vec2& uv) {}

    protected:
        const fresnelType fType;
        float ior;
        float k;
};

class lambertianBrdf : public bxdf {
    public:
        lambertianBrdf(const rgbColor& r) :
            bxdf(bxdfType(DIFFUSE | REFLECTION)), rOverPi(r * INVPI) {}
        lambertianBrdf(const rgbColor& r, texture2DPtr diffuseTex);

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

        virtual void updateFromUVTexture(const vec2& uv);
        rgbColor rOverPi;

    private:
};

class specularBrdf : public specularBxdf {
    public:
        specularBrdf(const float& i, const float& K, const fresnelType ft, rgbColor kr) :
            specularBxdf(bxdfType(SPECULAR | REFLECTION), i, K, ft), kR(kr)
        {}

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

    private:
        rgbColor kR;
};

class specularBtdf : public specularBxdf {
    public:
        specularBtdf(const float& i, const rgbColor& kt) :
            specularBxdf(bxdfType(SPECULAR | TRANSMISSION), i, 0.f, DIELECTRIC), kT(kt)
        {}

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

    private:
        rgbColor kT;
};

class phongBrdf : public bxdf {
    public:
        phongBrdf(const rgbColor& s, const float& N) :
            bxdf(bxdfType(GLOSSY | REFLECTION)), ks(s), n(N)
        {}

        virtual rgbColor f(const vec3& wo, const vec3& wi) const; 
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

        virtual void updateFromUVTexture(const vec2& uv) {}

    private:
        rgbColor ks;
        const float n;
};

class microfacetDistribution {
    public:
        microfacetDistribution(const rgbColor& r) : rho(r) {}

        virtual ~microfacetDistribution() {}

        virtual float D(const vec3& wh) const = 0;

        // Default is the Torrance-Sparrow shadowing masking function.
        virtual float G(const vec3& wo, const vec3& wi, const vec3& wh) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const = 0;
        virtual void sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const = 0;

        const rgbColor rho;
};

/**
 * Torrance/Sparrow microfacet BRDF model.
 */
class microfacetBrdf : public bxdf {
    public:
        microfacetBrdf(const float& e, const float& K, microfacetDistribution* d);
        virtual ~microfacetBrdf();

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

        virtual void updateFromUVTexture(const vec2& uv) {}

        rgbColor evalFresnel(const float& cosTheta) const;

        float eta, k;
        microfacetDistribution* distrib;
};

class microfacetBtdf : public bxdf {
    public:
        microfacetBtdf(const float& e, const float& K, microfacetDistribution* d);
        virtual ~microfacetBtdf();

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

        virtual void updateFromUVTexture(const vec2& uv) {}

        rgbColor evalFresnel(const float& cosTheta) const;

        float eta, k;
        microfacetDistribution* distrib;
};

class frostedGlassBsdf : public bsdf {
    public:
        frostedGlassBsdf(
                microfacetBrdf* reflDistrib,
                microfacetBtdf* transDistrib);

        virtual rgbColor f(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;
        virtual rgbColor sampleF(const float& u0, const float& u1, const float&
                u2, const vec3& wo, vec3& wi, bxdfType type, bxdfType& sampledType,
                float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;

    private:
        microfacetBrdf* glossRef;
        microfacetBtdf* glossTra;
};

/**
 * Bilnn microfacet distribution.
 */
class blinn : public microfacetDistribution {
    public:
        blinn(const rgbColor& r, const float& e) :
            microfacetDistribution(r), exp(e)
        {}

        virtual float D(const vec3& wh) const;
        virtual void sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

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

        virtual float D(const vec3& wh) const;
        virtual void sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

    private:
        float exponent(const vec3& wh) const;

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

        virtual float D(const vec3& wh) const;
        virtual float G(const vec3& wo, const vec3& wi, const vec3& wh) const;

        virtual void sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

    private:
        float G1(const vec3& v, const vec3& wh) const;
        float alpha;
};

class newWard : public bxdf {
	public:
		newWard(const rgbColor& rs, const float& a, const float& b) :
			bxdf(bxdfType(GLOSSY | REFLECTION)), Rs(rs), alpha(a), beta(b),
            isIsotropic(alpha==beta) {}

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;
        virtual float pdf(const vec3& wo, const vec3& wi) const;

        virtual void updateFromUVTexture(const vec2& uv) {}

	private:
		rgbColor Rs;
		float alpha, beta;
        bool isIsotropic;
};

class substrate : public bxdf {
    public:
        substrate(const rgbColor& rd, const rgbColor& rs, microfacetDistribution* d) : bxdf(bxdfType(GLOSSY | REFLECTION)),
            Rd(rd), Rs(rs), ecTerm(((28.f * rd) / (23.f * PI)) * rs.inverse()),
            distrib(d)
        {}

        ~substrate();

        virtual rgbColor f(const vec3& wo, const vec3& wi) const;
        virtual rgbColor sampleF(const float& u0, const float& u1,
                const vec3& wo, vec3& wi, float& pd) const;

        virtual float pdf(const vec3& wo, const vec3& wi) const;

        virtual void updateFromUVTexture(const vec2& uv) {}

    private:
        rgbColor Rd, Rs;
        rgbColor ecTerm;
        microfacetDistribution* distrib;
};

inline bool bsdf::isSubtype(bxdfType a, bxdfType b) {
    return ((a & b) == a);
}

inline float bsdf::cosTheta(const vec3& v){
    return v.y;
}

inline float bsdf::cos2Theta(const vec3& v){
    return v.y*v.y;
}

inline float bsdf::sinTheta(const vec3& v){
    return sqrt(std::max(0.f, 1.f - v.y*v.y));
}

inline float bsdf::sin2Theta(const vec3& v){
    const float c = cosTheta(v);
    return 1.f - c*c;
}
