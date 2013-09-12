#include "material.hpp"
#include "bsdf.hpp"
#include "mathlib/vector.hpp"

material::material(bsdf* bs) : b(bs), emissive(false), emitColor(0.f), emitPower(0.f)
{}

material::material(const rgbColor& c, const float& pow) : emissive(true),
		emitColor(c), emitPower(pow)
{
    bsdf* p = new bsdf();
    p->addBxdf(new lambertianBrdf(rgbColor(1.f)));
    b.reset(p);
}

rgbColor material::Le() const {
    if(emissive){
        return emitColor * emitPower;
    }else{
        return rgbColor(0.f);
    }
}
