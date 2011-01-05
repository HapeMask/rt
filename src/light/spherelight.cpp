#include "spherelight.hpp"
#include "geometry/sphere.hpp"

sphereLight::sphereLight(const point3& p, const float& pow, const rgbColor& c, const float& r) :
    light(p, pow, c), radius(r), radius2(r*r),
    area(4.f * PI * r*r), invArea(1.f/(4.f * PI * r*r))
{}

const float sphereLight::pdf(const point3& p, const vec3& wi) const {
    // P(wi) = r^2 / cosTheta * A
    const vec3 dir = normalize(wi);
    const intersection isect = intersect(ray(p, dir));
    if(isect.hit){
        // If it does hit, cosTheta will always be positive so no need for any
        // absolute value.
        const vec3 normal = getNormal(p + isect.t * dir);
        const float cosTheta = dot(-wi, normal);
        return (isect.t * isect.t) / (cosTheta * area);
    }else{
        return 0.f;
    }
}

const rgbColor sphereLight::sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const {
    // Create an arbitrary basis around w (direction towards the light) for the
    // transformation from that space to world space.
    //
    // NOTE: This method fails if the light is directly above the point (w =
    // <0,1,0>).
    //
    // TODO: Fix that.
    const vec3 w = normalize(position - p);
    const vec3 u = normalize(cross(w, vec3(0,1,0)));
    const vec3 v = normalize(cross(u, w));

    const float distanceTerm = sqrtf(1.f - radius2 / norm2(p-position));
    const float phi = TWOPI * u1;

    // Theta itself is never actually needed anywhere, so no need to calculate
    // it as specified in the paper.
    const float cosTheta = 1.f - u0 + u0*distanceTerm;
    const float sinTheta = sqrtf(1.f - cosTheta*cosTheta);
    //const float theta = acosf(cosTheta);

    // Note the swapping of w and v columns in the "matrix" below as compared
    // to the paper. This is because our coordinate system has Y-axis as the
    // "up" direction.
    const float v1 = sinTheta * cosf(phi);
    const float v2 = cosTheta;
    const float v3 = sinTheta * sinf(phi);
    const vec3 a = normalize(vec3(
                v1 * u.x + v2 * w.x + v3 * v.x,
                v1 * u.y + v2 * w.y + v3 * v.y,
                v1 * u.z + v2 * w.z + v3 * v.z));

    const intersection isect = intersect(ray(p, a));
    const point3 samplePoint = p + isect.t * a;
    const vec3 normal = getNormal(p);

    wi = samplePoint - p;
    pd = dot(-wi, normal) / ( TWOPI * isect.t * isect.t * (1.f - distanceTerm) );
    return lightColor * power;
}

const intersection sphereLight::intersect(const ray& r) const {
	const vec3 dir(r.origin - position);
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
            t = std::min(t0, t1);
        }else{
            t = t0;
        }
    }else{
        t = t1;
    }

    intersection isect(this, t);
    isect.normal = normalize(t * r.direction - position);
	return isect;
}

const bool sphereLight::intersectB(const ray& r) const {
	const vec3 dir(r.origin - position);
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
