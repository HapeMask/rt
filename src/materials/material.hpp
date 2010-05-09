#ifndef __RT_MATERIAL_
#define __RT_MATERIAL_

#include <tr1/memory>

#include "bsdf.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"

using namespace std;
using tr1::shared_ptr;

class material {
    public:
        material(bsdfPtr br);
        material(const rgbColor& c, const float& pow);

        const rgbColor Le() const;
        const rgbColor sample(const point3& p, const vec3& wi, const vec3& wo, bxdfType type) const;

        inline const bsdf& getBsdf() const {
            return *b.get();
        }

        inline const bool isEmissive(){
            return emissive;
        }

    protected:
        bsdfPtr b;
        bool emissive;
        rgbColor emitColor;
        float emitPower;
};

typedef shared_ptr<material> materialPtr;
#endif
