#include "material.hpp"
#include "bsdf.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"

material::material(bsdfPtr br) : b(br), emissive(false), emitColor(0.f), emitPower(0.f)
{}

material::material(const rgbColor& c, const float& pow) : emissive(true),
		emitColor(c), emitPower(pow)
{
    bsdf* p = new bsdf();
    p->addBxdf(new lambertianBrdf(1.f));
    b.reset(p);
}

const rgbColor material::sample(const point3& p, const vec3& wo, const vec3& wi, bxdfType type) const {
	if(!emissive){
		return b->f(wo, wi, type);
	}else{
		return Le();
	}
}

const rgbColor material::Le() const {
    if(emissive){
        return emitColor * emitPower;
    }else{
        return 0.f;
    }
}
