#include "triangle.hpp"
#include "shape.hpp"
#include "utility.hpp"

#include "scene/scene.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/ray.hpp"
#include "acceleration/intersection.hpp"

#include "samplers/samplers.hpp"

#include <cmath>

triangle::triangle(const point3& a, const point3& b, const point3& c) :
    primitive(aabb(
                    vec3(minps(minps(a.getSIMD(), b.getSIMD()), c.getSIMD())),
                    vec3(maxps(maxps(a.getSIMD(), b.getSIMD()), c.getSIMD()))
                )
            ), B(b-a), C(c-a), hasVertNormals(false), hasUVs(false) {

	points[0] = a;
	points[1] = b;
	points[2] = c;

    normal_ = cross(b-a, c-a);
    area_ = 0.5 * normal_.length();
	normal_ = normalize(normal_);
}

const intersection triangle::intersect(ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(abs(D) < EPSILON){
        return noIntersect;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - points[0];
    const float beta = dot(dir, s1) * invD;
    if(beta < 0.f || beta > 1.f){
        return noIntersect;
    }

    const vec3 s2 = cross(B, dir);
    const float gamma = dot(r.direction, s2) * invD;
    if(gamma < 0.f || beta + gamma > 1.f){
        return noIntersect;
    }

    const float t = dot(C, s2) * invD;
	if(t < r.tMin || t >= r.tMax){
		return noIntersect;
	}

	r.origin = r.origin + t * r.direction;

    intersection isect(this, t);
    isect.normal = normal_;

    const float alpha = 1.f - (beta + gamma);
    if(hasVertNormals){
        isect.shadingNormal = alpha * vertNormals[0] + beta * vertNormals[1] + gamma * vertNormals[2];
    }else{
        isect.shadingNormal = normal_;
    }

    if(hasUVs){
        isect.uv = alpha * uvs[0] + beta * uvs[1] + gamma * uvs[2];

        // Compute partial derivatives in u/v for the hit.
        const float du1 = uvs[0].x() - uvs[2].x();
        const float du2 = uvs[1].x() - uvs[2].x();
        const float dv1 = uvs[0].y() - uvs[2].y();
        const float dv2 = uvs[1].y() - uvs[2].y();

        const float invDet = 1.f / (du1*dv2 - dv1*du2);
        isect.binormal = (dv2*B - dv1*C) * invDet;
    }else{
        makeCoordinateSystem(isect.shadingNormal, isect.binormal, isect.tangent);
    }

	return isect;
}

const point3 triangle::sampleSurface(const float& u0, const float& u1) const {
    point3 ret;
    sampleTriangle(ret, *this, u0, u1);
    return ret;
}

void triangle::setVertNormals(const vec3& an, const vec3& bn, const vec3& cn){
    vertNormals[0] = an;
    vertNormals[1] = bn;
    vertNormals[2] = cn;
    hasVertNormals = true;
}
