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

const rgbColor lambertianBrdf::sampleF(const float& u1, const float& u2, const vec3& normal, const vec3& wo, vec3& wi) const{
    return rOverPi;
}

const rgbColor specularBrdf::sampleF(const float& u1, const float& u2, const vec3& normal, const vec3& wo, vec3& wi) const{
    if(reflective){
        wi = reflect(-wo, normal);
        return rgbColor(0,0,0);
    }else{
        const float cosTheta1 = dot(normal, -wo);
        const float nr = (cosTheta1 <= 0.f) ? (1.00029f / ior) : (ior / 1.00029f);
        const float sinSqTheta = (nr*nr) * (1.f - cosTheta1*cosTheta1);

        wi = (nr * -wo) - ( nr * cosTheta1 + sqrt(1.f - sinSqTheta))*normal;
        return rgbColor(0,0,0);
    }
}
