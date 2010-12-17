#include "bsdf.hpp"
#include "texture.hpp"
#include "color/color.hpp"

const bxdfType bxdf::getType() const {
    return type;
}

inline const bool bxdf::isType(const bxdfType t) const {
    return (type & t) == type;
}

void bxdf::setTexture(textureSlot slot, texture2DPtr p) {
    hasTexture = true;
    textureSlots[slot] = p;
}

const texture2D& bxdf::getTexture(textureSlot slot) const {
    return *textureSlots[slot];
}

const rgbColor bxdf::textureLookup(const textureSlot& slot, const vec2& uv) const {
    return textureSlots[slot]->lookup(uv);
}
