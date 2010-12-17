#pragma once

#include <tr1/memory>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "bsdf.hpp"

using namespace std;
using tr1::shared_ptr;

class material {
    public:
        material(bsdfPtr br);
        material(const rgbColor& c, const float& pow);

        const rgbColor Le() const;

        inline const bsdf& getBsdf() const {
            return *b.get();
        }

        inline const bsdf& getBsdf(const vec2& uv) {
            b->updateFromUVTexture(uv);
            return *b;
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
