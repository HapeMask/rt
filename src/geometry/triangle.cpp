#include "triangle.hpp"
#include "shape.hpp"

#include "utility.hpp"

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
            ), hasVertNormals(false) {

	points[0] = a;
	points[1] = b;
	points[2] = c;

	normal_ = normalize(cross(b - a, c - a));

	// Find the biggest normal axis, then remove that
	// coordinate from A, B, C to get a,b,cPrime.
    if(abs(normal_.x()) > abs(normal_.y())){
        if(abs(normal_.x()) > abs(normal_.z())){
            axis1 = 1;
            axis2 = 2;
        }else{
            axis1 = 0;
            axis2 = 1;
        }
    }else{
        if(abs(normal_.y()) > abs(normal_.z())){
            axis1 = 0;
            axis2 = 2;
        }else{
            axis1 = 0;
            axis2 = 1;
        }
    }

	aPrime = vec2(a(axis1), a(axis2));
	bPrime = vec2(b(axis1), b(axis2)) - aPrime;
	cPrime = vec2(c(axis1), c(axis2)) - aPrime;
}

const intersection triangle::intersect(ray& r) const {
	const float D = dot(normal_, r.direction);
	const float t = -dot(r.origin - a(), normal_) / D;

	if(t < r.tMin || t >= r.tMax){
		return noIntersect;
	}

	// Determine the potential intersection point.
	const point3 pI = r.origin + (t * r.direction);

	const vec2 pPrime = vec2(pI(axis1), pI(axis2)) - aPrime;

	const float dBeta = bPrime.y()*cPrime.x() - bPrime.x()*cPrime.y();
	const float dGamma = -dBeta;

    /*
	if(abs(dBeta) < EPSILON){
		return noIntersect;
	}
    */

	// Calculate the barycentric coordinates of the potential intersection.
	const float beta = (pPrime.y()*cPrime.x() - pPrime.x()*cPrime.y()) / dBeta;
	const float gamma = (pPrime.y() * bPrime.x() - pPrime.x()*bPrime.y()) / dGamma;

	if(beta < 0.f || gamma < 0.f || beta+gamma > 1.f){
		return noIntersect;
	}

	r.origin = pI;
    intersection isect(parent, this, t);
    isect.normal = normal_;
    makeCoordinateSystem(isect.normal, isect.dpdu, isect.dpdv);
    if(!hasVertNormals){
        isect.shadingNormal = normal_;
    }else{
        const float alpha = 1.f - (beta + gamma);
        isect.shadingNormal = alpha * vertNormals[0] + beta * vertNormals[1] + gamma * vertNormals[2];
        makeCoordinateSystem(isect.shadingNormal, isect.dsdu, isect.dsdv);
    }
	return isect;
}

const point3 triangle::uniformSampleSurface() const{
    point3 ret;
    uniformSampleTriangle(ret, *this);
    return ret;
}

void triangle::setVertNormals(const vec3& an, const vec3& bn, const vec3& cn){
    vertNormals[0] = an;
    vertNormals[1] = bn;
    vertNormals[2] = cn;
    hasVertNormals = true;
}
