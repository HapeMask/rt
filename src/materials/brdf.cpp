#include "brdf.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"

lambertianBrdf::lambertianBrdf(const rgbColor& r) : brdf(false), rOverPi(r / PI)
{}

const rgbColor lambertianBrdf::f(const vec3& wi, const vec3& wo) const {
	return rOverPi;
}

const vec3 lambertianBrdf::sampleF(const float& u1, const float& u2, const vec3& wo, const vec3& normal) const{
    return wo;
}

const vec3 specularBrdf::sampleF(const float& u1, const float& u2, const vec3& wo, const vec3& normal) const{
    if(reflective){
        return reflect(-wo, normal);
    }else{
        const float cosTheta1 = dot(normal, -wo);
        const float nr = (cosTheta1 < 0.f) ? (1.00029f / ior) : (ior / 1.00029f);
        const float sinSqTheta = (nr*nr) * (1.f - cosTheta1*cosTheta1);

        return (nr * -wo) - ( nr * cosTheta1 + sqrt(1.f - sinSqTheta))*normal;
    }
}
