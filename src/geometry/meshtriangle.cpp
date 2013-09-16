#include "meshtriangle.hpp"
#include "shape.hpp"
#include "utility.hpp"

#include "scene/scene.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "acceleration/intersection.hpp"

#include "samplers/samplers.hpp"

#include <cmath>

meshTriangle::meshTriangle(const int& _a, const int& _b, const int& _c, triangleMesh* p) :
    primitive(aabb(
                min(min(p->pointLookup(_a), p->pointLookup(_b)), p->pointLookup(_c)),
                max(max(p->pointLookup(_a), p->pointLookup(_b)), p->pointLookup(_c))
                ), p),
    points{_a,_b,_c}, hasVertNormals(false), hasUVs(false)
{

    const point3& pa = a();
    const point3& pb = b();
    const point3& pc = c();

    B = pb - pa;
    C = pc - pa;

    normal_ = cross(B, C);
    area_ = 0.5 * norm(normal_);
    normal_ = normalize(normal_);
}

intersection meshTriangle::intersect(ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(abs(D) < EPSILON){
        return noIntersect;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - a();
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
        isect.shadingNormal =
            alpha * vertNormal(0) +
            beta * vertNormal(1) +
            gamma * vertNormal(2);
    }else{
        isect.shadingNormal = normal_;
    }

    if(hasUVs){
        // Interpolate the UV coordinates using the barycentric hitpoint
        // coordinates.
        isect.uv = alpha * uv(0) + beta * uv(1) + gamma * uv(2);

        isect.binormal = binormal_;
        isect.tangent = normalize(cross(isect.shadingNormal, binormal_));
    }else{
        makeCoordinateSystem(isect.shadingNormal, isect.binormal, isect.tangent);
    }

    return isect;
}

bool meshTriangle::intersectB(const ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(abs(D) < EPSILON){
        return false;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - a();
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

void meshTriangle::setVertNormals(const int& an, const int& bn, const int& cn){
    vertNormals[0] = an;
    vertNormals[1] = bn;
    vertNormals[2] = cn;
    hasVertNormals = true;
}

void meshTriangle::setUVs(const int& auv, const int& buv, const int& cuv){
    uvs[0] = auv;
    uvs[1] = buv;
    uvs[2] = cuv;
    hasUVs = true;

    const vec2 d1 = uv(0) - uv(2);
    const vec2 d2 = uv(1) - uv(2);

    // Compute partial derivatives in u/v for the hit.
    const float invDetUV = 1.f / (d1.u*d2.v - d1.v*d2.u);
    binormal_ = normalize((d2.v*B - d1.v*C) * invDetUV);
}

#ifdef RT_USE_QT
void meshTriangle::prepGL(GLfloat*& vertexData, GLfloat*& normalData) const {
    for(int i=0; i<3; ++i){
        if(hasVertNormals){
            (*normalData) = vertNormal(i).x; ++normalData;
            (*normalData) = vertNormal(i).y; ++normalData;
            (*normalData) = vertNormal(i).z; ++normalData;
        }else{
            (*normalData) = normal_.x; ++normalData;
            (*normalData) = normal_.y; ++normalData;
            (*normalData) = normal_.z; ++normalData;
        }

        (*vertexData) = parentMesh()->pointLookup(points[i]).x; ++vertexData;
        (*vertexData) = parentMesh()->pointLookup(points[i]).y; ++vertexData;
        (*vertexData) = parentMesh()->pointLookup(points[i]).z; ++vertexData;
    }
}
#endif
