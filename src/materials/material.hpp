#ifndef __RT_MATERIAL_
#define __RT_MATERIAL_

#include <tr1/memory>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "brdf.hpp"

using namespace std;
using tr1::shared_ptr;

class material {
public:
	material(brdfPtr br);
	material(const rgbColor& c, const float& pow);

	const rgbColor sample(const point3& p, const vec3& wi, const vec3& wo) const;
	const rgbColor sampleL() const;

    inline const rgbColor sampleF(const float& u1, const float& u2, const vec3& normal, const vec3& wo, vec3& wi) const{
        return b->sampleF(u1, u2, normal, wo, wi);
    }

	const bool& isEmissive(){
		return emissive;
	}

	const bool& isSpecular(){
		return b->isSpecular();
	}

protected:
	brdfPtr b;
	bool emissive;
	rgbColor emitColor;
	float emitPower;
};

typedef shared_ptr<material> materialPtr;
#endif
