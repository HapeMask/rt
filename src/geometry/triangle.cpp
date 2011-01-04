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
    area_ = 0.5 * (float)norm(normal_);
	normal_ = normalize(normal_);
}

const intersection triangle::intersect(ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(fabsf(D) < EPSILON){
        return noIntersect;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - a_;
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
	if(t < r.tMin || t > r.tMax){
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
        isect.tangent = cross(isect.shadingNormal, binormal_);
        isect.binormal = cross(isect.tangent, isect.shadingNormal);
    }else{
        isect.uv = vec2(beta, gamma);
        makeCoordinateSystem(isect.shadingNormal, isect.binormal, isect.tangent);
    }
	return isect;
}

const bool triangle::intersectB(const ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(abs(D) < EPSILON){
        return false;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - a_;
    const float beta = dot(dir, s1) * invD;
    if(beta < 0.f || beta > 1.f){
        return false;
    }

    const vec3 s2 = cross(B, dir);
    const float gamma = dot(r.direction, s2) * invD;
    if(gamma < 0.f || beta + gamma > 1.f){
        return false;
    }

    const float t = dot(C, s2) * invD;
	if(t < r.tMin || t >= r.tMax){
		return false;
	}

	return true;
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

void triangle::prepGL(GLfloat*& data) const {
    for(int i=0; i<3; ++i){
        if(hasVertNormals){
            (*data) = vertNormals[i].x;
            ++data;
            (*data) = vertNormals[i].y;
            ++data;
            (*data) = vertNormals[i].z;
            ++data;
        }else{
            (*data) = normal_.x;
            ++data;
            (*data) = normal_.y;
            ++data;
            (*data) = normal_.z;
            ++data;
        }

        const point3& p = (i == 0) ? a_ : ((i == 1) ? b_ : c_);

        (*data) = p.x;
        ++data;
        (*data) = p.y;
        ++data;
        (*data) = p.z;
        ++data;
    }
}

void triangle::drawGL() const {
    // Do nothing. This triangle's data has been dumped to the scene VBO
    // already and will be drawn as such.
}
