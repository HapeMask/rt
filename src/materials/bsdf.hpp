#ifndef __RT_BRDF_
#define __RT_BRDF_

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include <tr1/memory>
using std::tr1::shared_ptr;

enum bxdfType {
    REFLECTION =    1<<0,
    TRANSMISSION =  1<<1,
    DIFFUSE =       1<<2,
    GLOSSY =        1<<3,
    SPECULAR =      1<<4
};

enum fresnelType {
    CONDUCTOR = 0,
    DIELECTRIC = 1
};

class bxdf {
    public:
        bxdf(const bxdfType t) : type(t) {}

        virtual const rgbColor f(const vec3& wo, const vec3& wi) const = 0;
        virtual const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const = 0;
        virtual ~bxdf() {}

        const bxdfType getType() const {
            return type;
        }

        inline const bool isType(const bxdfType t) const {
            return (type & t) == type;
        }

    private:
        bxdfType type;
};

class bsdf {
    public:
        bsdf() : specTra(NULL), specRef(NULL),
        diffTra(NULL), diffRef(NULL),
        glossTra(NULL), glossRef(NULL) {}
        ~bsdf();

        virtual const rgbColor f(const vec3& wo, const vec3& wi, bxdfType type) const;
        const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi, bxdfType type) const;
        void addBxdf(bxdf* b);

    private:
        bxdf *specTra, *specRef,
             *diffTra, *diffRef,
             *glossTra, *glossRef;
};

class lambertianBrdf : public bxdf {
    public:
        lambertianBrdf(const rgbColor& r) : bxdf(bxdfType(REFLECTION | DIFFUSE)), rOverPi(r / PI) {}
        virtual ~lambertianBrdf() {}

        virtual const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const;

    private:
        rgbColor rOverPi;
};

class specularBrdf : public bxdf {
    public:
        specularBrdf(const float& i, const fresnelType ft) : bxdf(bxdfType(TRANSMISSION | SPECULAR)), fType(ft), ior(i)
        {}
        virtual ~specularBrdf() {}

        virtual const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const;
        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            return rgbColor(0.f);
        }

    private:
        fresnelType fType;
        float ior;
};

class specularBtdf : public bxdf {
    public:
        specularBtdf(const float& i, const fresnelType ft) : bxdf(bxdfType(TRANSMISSION | SPECULAR)), fType(ft), ior(i)
        {}
        virtual ~specularBtdf() {}

        virtual const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const;
        inline virtual const rgbColor f(const vec3& wo, const vec3& wi) const {
            return rgbColor(0.f);
        }

    private:
        fresnelType fType;
        float ior;
};

class phongBrdf : public bxdf {
    public:
        phongBrdf(const float& N) : bxdf(bxdfType(REFLECTION | SPECULAR)), n(N)
        {}

        virtual const rgbColor sampleF(const float& u1, const float& u2, const vec3& wo, vec3& wi) const;
        virtual const rgbColor f(const vec3& wo, const vec3& wi) const;

    private:
        float n;
}

typedef shared_ptr<bsdf> bsdfPtr;
typedef shared_ptr<bxdf> bxdfPtr;
#endif
