#include "utility.hpp"
#include "camera.hpp"

#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
#include "mathlib/matrix.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/transformation.hpp"

#include <cmath>

camera::camera(const float& w, const float& h, const float& nr, const float& fr,
				const float& fov, const point3& pos, const point3& look, const vec3& up) : _width(w), _height(h) {
	float screen[4];
	if(w > h){
		const float ratio = (float)w / (float)h;
		screen[0] = -ratio;
		screen[1] = ratio;
		screen[2] = -1;
		screen[3] = 1;
	}else{
		const float ratio = (float)h / (float)w;
		screen[0] = -1;
		screen[1] = 1;
		screen[2] = -ratio;
		screen[3] = ratio;
	}

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

const ray camera::getRay(const float& x, const float& y){
	const point3 rasterPos(x, y, 0.f);
	const point3 cameraPos = rasterToCamera.apply(rasterPos);

    return worldToCamera.unapply(ray(cameraPos, normalize(vec3(cameraPos))));
}
