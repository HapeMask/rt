#include "triangle.hpp"
#include "shape.hpp"

#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/ray.hpp"

#include <cmath>

triangle::triangle(const point3& a, const point3& b, const point3& c, shape* parent) :
	primitive(parent){

	boundingBox = aabb(
			max(max(a.y(), b.y()), c.y()), // Top
			min(min(a.y(), b.y()), c.y()), // Bottom
			min(min(a.x(), b.x()), c.x()), // Left
			max(max(a.x(), b.x()), c.x()), // Right
			min(min(a.z(), b.z()), c.z()), // Front
			max(max(a.z(), b.z()), c.z()) // Back
			);

	points[0] = a;
	points[1] = b;
	points[2] = c;

	normal_ = normalize(cross(b - a, c - a));

	// Find the biggest normal axis, then remove that
	// coordinate from A, B, C to get a,b,cPrime.
	const float maxD = max(max(abs(normal_(0)), abs(normal_(1))), abs(normal_(2)));
	if(maxD == normal_(0)){
		axis1 = 1;
		axis2 = 2;
	}else if(maxD == normal_(1)){
		axis1 = 0;
		axis2 = 2;
	}else{
		axis1 = 0;
		axis2 = 1;
	}

	aPrime = vec2(a(axis1), a(axis2));
	bPrime = vec2(b(axis1), b(axis2)) - aPrime;
	cPrime = vec2(c(axis1), c(axis2)) - aPrime;
}

const bool triangle::intersect(ray& r) const {
	const float D = dot(normal_, r.direction);

	/* Backface culling, perpendicularity test.
	 * Can be uncommented for a speedup if reflections
	 * aren't desired.
	if(D >= 0){
		return false;
	}
	*/

	const float t = -dot(r.origin - a(), normal_) / D;
	if(t <= r.tMin || t >= r.tMax){
		return false;
	}

	// Determine the potential intersection point.
	const point3 pI = r.origin + (t * r.direction);

	const vec2 pPrime = vec2(pI(axis1), pI(axis2)) - aPrime;

	const float dBeta = bPrime.y()*cPrime.x() - bPrime.x()*cPrime.y();
	const float dGamma = -dBeta;

	if(abs(dBeta) <= EPSILON){
		return false;
	}

	// Calculate the barycentric coordinates of the potential intersection.
	const float beta = (pPrime.y()*cPrime.x() - pPrime.x()*cPrime.y()) / dBeta;
	const float gamma = (pPrime.y() * bPrime.x() - pPrime.x()*bPrime.y()) / dGamma;

	if(beta < -EPSILON || gamma < -EPSILON || beta+gamma > 1.f + EPSILON){
		return false;
	}

	r.origin = pI;
	return true;
}
