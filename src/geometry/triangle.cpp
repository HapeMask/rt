#include "triangle.hpp"
#include "shape.hpp"
#include "utility.hpp"

#include "scene/scene.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "acceleration/intersection.hpp"

#include "samplers/samplers.hpp"

#include <cmath>

triangle::triangle(const point3& a, const point3& b, const point3& c) :
    primitive(aabb(min(min(a, b), c), max(max(a, b), c))),
    B(b-a), C(c-a), hasVertNormals(false), hasUVs(false) {

	a_ = a;
	b_ = b;
	c_ = c;

    normal_ = cross(b-a, c-a);
    area_ = 0.5 * norm(normal_);
	normal_ = normalize(normal_);
}

intersection triangle::intersect(ray& r) const {
    const vec3 v1 = cross(C, r.direction);
    const float D = dot(v1, B);
    const float invD = 1.f / D;

    const vec3 dir = r.origin - a_;
    const vec3 v2 = cross(B, dir);

    const float beta = dot(dir, v1) * invD;
    const float gamma = dot(r.direction, v2) * invD;

    const float t = dot(C, v2) * invD;

    if(abs(D) > EPSILON &&
            beta >= 0.f && beta <= 1.f &&
            gamma >= 0.f && beta+gamma <= 1.f &&
            t >= r.tMin && t < r.tMax){

        r.origin = r.origin + t * r.direction;

        intersection isect(this, t);
        isect.normal = normal_;

        isect.shadingNormal = normal_;
        isect.uv = vec2(beta, gamma);
        makeCoordinateSystem(isect.shadingNormal, isect.binormal, isect.tangent);
        return isect;
    }else{
        return noIntersect;
    }
}

bool triangle::intersectB(const ray& r) const {
    const vec3 v1 = cross(C, r.direction);
    const float D = dot(v1, B);
    const float invD = 1.f / D;
    const vec3 dir = r.origin - a_;

    const vec3 v2 = cross(B, dir);

    const float beta = dot(dir, v1) * invD;
    const float gamma = dot(r.direction, v2) * invD;

    const float t = dot(C, v2) * invD;

    return (abs(D) > EPSILON &&
            beta >= 0.f && beta <= 1.f &&
            gamma >= 0.f && beta+gamma <= 1.f &&
            t >= r.tMin && t < r.tMax);
}

void triangle::setUVs(const vec2& auv, const vec2& buv, const vec2& cuv){
    uvs[0] = auv;
    uvs[1] = buv;
    uvs[2] = cuv;
    hasUVs = true;

    // Compute partial derivatives in u/v for the hit.
    const float du1 = uvs[0].x - uvs[2].x;
    const float du2 = uvs[1].x - uvs[2].x;
    const float dv1 = uvs[0].y - uvs[2].y;
    const float dv2 = uvs[1].y - uvs[2].y;
    const float invDetUV = 1.f / (du1*dv2 - dv1*du2);

    const vec3 dp1 = a_ - c_;
    const vec3 dp2 = b_ - c_;

    binormal_ = normalize((dv2 *  dp1 - dv1 * dp2) * invDetUV);
    tangent_ = normalize((-du2 *  dp1 + du1 * dp2) * invDetUV);
}

void triangle::prepGL(GLfloat*& vertexData, GLfloat*& normalData) const {
    for(int i=0; i<3; ++i){
        if(hasVertNormals){
            (*normalData) = vertNormals[i].x;
            ++normalData;
            (*normalData) = vertNormals[i].y;
            ++normalData;
            (*normalData) = vertNormals[i].z;
            ++normalData;
        }else{
            (*normalData) = normal_.x;
            ++normalData;
            (*normalData) = normal_.y;
            ++normalData;
            (*normalData) = normal_.z;
            ++normalData;
        }

        const point3& p = (i == 0) ? a_ : ((i == 1) ? b_ : c_);

        (*vertexData) = p.x;
        ++vertexData;
        (*vertexData) = p.y;
        ++vertexData;
        (*vertexData) = p.z;
        ++vertexData;
    }
}
