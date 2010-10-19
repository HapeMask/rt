#include "light.hpp"
#include "geometry/sphere.hpp"

sphereLight::sphereLight(const point3& p, const float& pow, const rgbColor& c, const float& r) :
    light(p, pow, c), radius(r), area(4.f * PI * r*r), invArea(1.f/(4.f * PI * r*r))
{}

const float sphereLight::pdf(const point3& p, const vec3& wi) const {
    // P(wi) = r^2 / cosTheta * A
    const vec3 dir = normalize(wi);
    const intersection isect = intersect(ray(p, dir));
    if(isect.hit){
        const vec3 normal = getNormal(p + isect.t * dir);
        const float cosTheta = fabs(dot(-wi, normal));
        if(cosTheta > 0.f){
            return (isect.t * isect.t) / (cosTheta * area);
        }else{
            return 0.f;
        }
    }else{
        return 0.f;
    }
}

const rgbColor sphereLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const {
    const point3 samplePoint = uniformSampleSurface();

    wi = samplePoint - p;
    pd = pdf(p, wi);
    return lightColor * power;
}

const intersection sphereLight::intersect(const ray& r) const {
	const vec3 dir(r.origin - location);
	const float A = dot(r.direction, r.direction);
	const float B = dot(2.f*dir, r.direction);
	const float C = dot(dir, dir) - (radius*radius);
	const float s = (B*B - 4*A*C);

	if(s < 0.f){
		return noIntersect;
	}

    const float q = (B < 0) ? (-B + sqrt(s)) / 2.f : (-B - sqrt(s)) / 2.f;;
	const float t0 = q / A;
	const float t1 = C / q;

	if( (t0 <= r.tMin || t0 >= r.tMax) &&
		(t1 <= r.tMin || t1 >= r.tMax)){
        return noIntersect;
    }

    float t = 0.f;
    if(t0 >= r.tMin && t0 <= r.tMax && t0 > 0){
        if(t1 >= r.tMin && t1 <= r.tMax && t1 > 0){
            t = min(t0, t1);
        }else{
            t = t0;
        }
    }else{
        t = t1;
    }

    intersection isect(this, t);
    isect.normal = normalize(t * r.direction - location);
	return isect;
}

const bool sphereLight::intersectB(const ray& r) const {
	const vec3 dir(r.origin - location);
	const float A = dot(r.direction, r.direction);
	const float B = dot(2.f*dir, r.direction);
	const float C = dot(dir, dir) - (radius*radius);
	const float s = (B*B - 4*A*C);

	if(s < 0.f){
		return false;
	}

    const float q = (B < 0) ? (-B + sqrt(s)) / 2.f : (-B - sqrt(s)) / 2.f;;
	const float t0 = q / A;
	const float t1 = C / q;

	if( (t0 <= r.tMin || t0 >= r.tMax) &&
		(t1 <= r.tMin || t1 >= r.tMax)){
        return false;
    }

    return true;
}
