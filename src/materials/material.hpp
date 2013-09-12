#pragma once
#include <memory>

#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "bsdf.hpp"

using std::shared_ptr;

class material {
    public:
        material(bsdf* br);
        material(const rgbColor& c, const float& pow);

        rgbColor Le() const;

        inline const bsdf& getBsdf() const {
            return *b.get();
        }

        inline const bsdf& getBsdf(const vec2& uv) {
            b->updateFromUVTexture(uv);
            return *b;
        }

        inline const bool& isEmissive(){
            return emissive;
        }

    protected:
        unique_ptr<bsdf> b;
        bool emissive;
        rgbColor emitColor;
        float emitPower;
};

typedef shared_ptr<material> materialPtr;
