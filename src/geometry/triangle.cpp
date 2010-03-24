#include "triangle.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/ray.hpp"

bool triangle::intersect(ray& r, point3& p){
	const float D = dot(normal_, r.direction());

	// Backface culling, perpendicularity test.
	/*
	if(D >= 0){
		return false;
	}
	*/

	const float t = -dot(r.origin - a(), normal_) / D;
	if(t <= EPSILON){
		return false;
	}

	const point3 pI = r.origin + (t * r.direction);

	const vec2 pPrime = vec2(pI(axis1), pI(axis2)) - aPrime;

	const float dBeta = bPrime.y()*cPrime.x() - bPrime.x()*cPrime.y();
	const float dGamma = -dBeta;

	if(abs(dBeta) <= EPSILON){
		return false;
	}

	const float beta = (pPrime.y()*cPrime.x() - pPrime.x()*cPrime.y()) / dBeta;
	const float gamma = (pPrime.y() * bPrime.x() - pPrime.x()*bPrime.y()) / dGamma;

	if(beta < -EPSILON || gamma < -EPSILON || beta+gamma > 1.f + EPSILON){
		return false;
	}

	p = pI;
	return true;
}
