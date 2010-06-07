#include "meshtriangle.hpp"
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

meshTriangle::meshTriangle(const unsigned int& a, const unsigned int& b, const unsigned int& c, triangleMesh* p) :
    primitive(aabb(
                    vec3(minps(minps(p->pointLookup(a).getSIMD(), p->pointLookup(b).getSIMD()), p->pointLookup(c).getSIMD())),
                    vec3(maxps(maxps(p->pointLookup(a).getSIMD(), p->pointLookup(b).getSIMD()), p->pointLookup(c).getSIMD()))
                ), p
            ), hasVertNormals(false), hasUVs(false), meshParent(p) {

	points[0] = a;
	points[1] = b;
	points[2] = c;

    const point3& pa = p->pointLookup(a);
    const point3& pb = p->pointLookup(b);
    const point3& pc = p->pointLookup(c);
    B = pb-pa;
    C = pc-pa;

    normal_ = cross(B, C);

    area_ = 0.5 * normal_.length();
	normal_ = normalize(normal_);
}

const intersection meshTriangle::intersect(ray& r) const {
    const vec3 s1 = cross(C, r.direction);
    const float D = dot(s1, B);
    if(abs(D) < EPSILON){
        return noIntersect;
    }

    const float invD = 1.f / D;

    const vec3 dir = r.origin - meshParent->pointLookup(points[0]);
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
            alpha * meshParent->vertNormalLookup(vertNormals[0]) +
            beta * meshParent->vertNormalLookup(vertNormals[1]) +
            gamma * meshParent->vertNormalLookup(vertNormals[2]);
    }else{
        isect.shadingNormal = normal_;
    }

    if(hasUVs){
        const vec2& uv1 = meshParent->uvLookup(uvs[0]);
        const vec2& uv2 = meshParent->uvLookup(uvs[1]);
        const vec2& uv3 = meshParent->uvLookup(uvs[2]);

        // Interpolate the UV coordinates using the barycentric hitpoint
        // coordinates.
        isect.uv = alpha * uv1 + beta * uv2 + gamma * uv3;

        isect.binormal = binormal_;
        isect.tangent = normalize(cross(isect.shadingNormal, binormal_));
    }else{
        makeCoordinateSystem(isect.shadingNormal, isect.binormal, isect.tangent);
    }

	return isect;
}

const point3 meshTriangle::sampleSurface(const float& u0, const float& u1) const {
    /*
    point3 ret;
    sampleTriangle(ret, *this, u0, u1);
    return ret;
    */
    cerr << "ERROR: NOT IMPLEMENTED." << endl;
    exit(1);
}

void meshTriangle::setVertNormals(const unsigned int& an, const unsigned int& bn, const unsigned int& cn){
    vertNormals[0] = an;
    vertNormals[1] = bn;
    vertNormals[2] = cn;
    hasVertNormals = true;
}

void meshTriangle::setUVs(const unsigned int& auv, const unsigned int& buv, const unsigned int& cuv){
    uvs[0] = auv;
    uvs[1] = buv;
    uvs[2] = cuv;
    hasUVs = true;

    const vec2& uv1 = meshParent->uvLookup(auv);
    const vec2& uv2 = meshParent->uvLookup(buv);
    const vec2& uv3 = meshParent->uvLookup(cuv);

    // Compute partial derivatives in u/v for the hit.
    du1 = uv1.x() - uv3.x();
    du2 = uv2.x() - uv3.x();
    dv1 = uv1.y() - uv3.y();
    dv2 = uv2.y() - uv3.y();
    invDetUV = 1.f / (du1*dv2 - dv1*du2);
    binormal_ = normalize((dv2*B - dv1*C) * invDetUV);
}
