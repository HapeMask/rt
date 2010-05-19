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

class lambertianBrdf : public bxdf {
    public:
        lambertianBrdf(const rgbColor& r) : bxdf(bxdfType(REFLECTION | DIFFUSE)), rOverPi(r / PI) {}

        virtual const rgbColor f(const vec3& wo, const vec3& wi) const;

    private:
        rgbColor rOverPi;
};

class specularBrdf : public bxdf {
    public:
        specularBrdf(const float& i, const fresnelType ft) : bxdf(bxdfType(TRANSMISSION | SPECULAR)), fType(ft), ior(i)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            return 0.f;
        }

        inline virtual const float pdf(const vec3& wo, const vec3& wi) const{
            return 0.f;
        }

    private:
        fresnelType fType;
        float ior;
};

class specularBtdf : public bxdf {
    public:
        specularBtdf(const float& i, const fresnelType ft) : bxdf(bxdfType(TRANSMISSION | SPECULAR)), fType(ft), ior(i)
        {}

        virtual const rgbColor sampleF(const float& u0, const float& u1, const vec3& wo, vec3& wi, float& pd) const;
        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            return 0.f;
        }

        inline virtual const float pdf(const vec3& wo, const vec3& wi) const {
            return 0.f;
        }

    private:
        fresnelType fType;
        float ior;
};

class phongBrdf : public bxdf {
    public:
        phongBrdf(const rgbColor& k, const float& N) : bxdf(bxdfType(REFLECTION | GLOSSY)), ks(k), n(N)
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
