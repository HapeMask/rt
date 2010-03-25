#include "camera.hpp"

#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
#include "mathlib/matrix.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/utility.hpp"
#include "mathlib/transformation.hpp"

#include <cmath>

camera::camera(const float& w, const float& h, const float scr[4], const float& nr, const float& fr,
				const float& fov, const point3& pos, const point3& look, const vec3& up){
	screen[0] = scr[0];
	screen[1] = scr[1];
	screen[2] = scr[2];
	screen[3] = scr[3];

	near = nr;
	far = fr;

	worldToCamera = lookAt(pos, look, up);
	cameraToScreen = perspective(fov, near, far);

	screenToRaster = transform3d(
			scale(w, h, 1.f) *
			scale(
				1.f / (screen[1] - screen[0]),
				1.f / (screen[2] - screen[3]), 1.f) *
			translate(-screen[0], -screen[3], 0.f)
		);

	rasterToCamera = transform3d(cameraToScreen.inverse() * screenToRaster.inverse());
}

void camera::getRay(const int& x, const int& y, ray& r){
	const point3 rasterPos(x, y, 0.f);
	const point3 cameraPos = rasterToCamera.apply(rasterPos);

	r.origin = cameraPos;
	r.direction = normalize(vec3(cameraPos));
	r = worldToCamera.reverse(r);
}
