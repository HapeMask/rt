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

        // The normal always points out of the object (as in PBRT), thus
        // we need to check if the ray is inside the object by checking
        // the sign of the angle between the ray and the normal.
        //
        // Swaps IOR accordingly and generates a refraction normal
        // that always points in the same direction as the ray.
        const float nr = (cosTheta1 <= 0.f) ? (1.f / ior) : (ior / 1.f);
        const vec3 refractionNormal = (cosTheta1 <= 0.f) ? normal : -normal;

        const float sinSqTheta = nr * nr * (1.f - cosTheta1*cosTheta1);
        if(sinSqTheta > 1.f){
            return rgbColor(-1,-1,-1);
        }

        wi = normalize((nr * -wo) + (nr * cosTheta1 - sqrt(1.f - sinSqTheta)) * refractionNormal);
        return rgbColor(0,0,0);
    }
}
