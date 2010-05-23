#ifndef __RT_BRDF_
#define __RT_BRDF_

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

inline const float rescaledSchlickFresnel(const float& eta, const float& k, const float& cosTheta){
    return ((eta-1.f)*(eta-1.f) + (4 * eta * pow(1.f - cosTheta, 5) + k*k)) /
        ((eta+1.f)*(eta+1.f) + k*k);
}

inline const vec3 halfVector(const vec3& wo, const vec3& wi){
    return normalize(wo + wi);
}

class bxdf;
class bsdf {
    public:
        bsdf() : specTra(NULL), specRef(NULL),
        diffTra(NULL), diffRef(NULL),
        glossTra(NULL), glossRef(NULL) {}
        ~bsdf();

        virtual const rgbColor f(const vec3& wo, const vec3& wi, bxdfType type = ALL) const;
        const rgbColor sampleF(const float& u0, const float& u1, const float& u2,
                const vec3& wo, vec3& wi, bxdfType type, bxdfType& matchedType) const;

        const rgbColor sampleF(const float& u0, const float& u1, const float& u2,
                const vec3& wo, vec3& wi, bxdfType type, bxdfType& matchedType, float& pd) const;

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
        bxdf *specTra, *specRef,
             *diffTra, *diffRef,
             *glossTra, *glossRef;
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
        bxdfType type;
};

class specularBxdf : public bxdf {
    public:
        specularBxdf(const bxdfType type, const float& eta, const float& K, const fresnelType ft) :
            bxdf(type), fType(ft), ior(eta), k(K) {}

        const float evalFresnel(const float& cosTheta) const {
            return rescaledSchlickFresnel(ior, k, cosTheta);
        }

    protected:
        fresnelType fType;
        float ior, k;
};

class lambertianBrdf : public bxdf {
    public:
        lambertianBrdf(const rgbColor& r) : bxdf(bxdfType(DIFFUSE | REFLECTION)), rOverPi(r * INVPI) {}

        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const{
            return rOverPi;
        }

    private:
        rgbColor rOverPi;
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
        rgbColor kR;
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
        rgbColor kT;
};

class phongBrdf : public bxdf {
    public:
        phongBrdf(const rgbColor& k, const float& N) : bxdf(N < 1000 ? bxdfType(GLOSSY | REFLECTION) : bxdfType(SPECULAR | REFLECTION)), ks(k), n(N)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const; 

        virtual const float pdf(const vec3& wo, const vec3& wi) const;

    private:
        rgbColor ks;
        float n;
};

typedef shared_ptr<bsdf> bsdfPtr;
typedef shared_ptr<bxdf> bxdfPtr;
#endif
