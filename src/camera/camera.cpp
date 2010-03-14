#include "camera.hpp"
#include "mathlib/ray.hpp"
#include <cmath>

void camera::getRay(const int& x, const int& y, ray& r){
	const vec4 rvec = vec4(vec3(x, y, 0.f), 1.f);
	vec4 cameraSpacePos = rasterToCamera * rvec;
	float w = cameraSpacePos.w();
	cameraSpacePos /= w;

	vec4 worldSpacePos = cameraToWorld * cameraSpacePos;
	w = worldSpacePos.w();
	worldSpacePos /= w;

	vec4 worldSpaceDirection = cameraToWorld * normalize(cameraSpacePos);
	w = worldSpaceDirection.w();
	worldSpaceDirection /= w;

	r.origin() = point3(worldSpacePos.x(), worldSpacePos.y(), worldSpacePos.z());
	r.direction() = normalize(vec3(worldSpaceDirection.x(), worldSpaceDirection.y(), -worldSpaceDirection.z()));
}
