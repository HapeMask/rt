#ifndef __RT_CAMERA__
#define __RT_CAMERA__

#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
#include "mathlib/constants.hpp"

#include <cmath>

class camera {
	public:
		camera(const float& w, const float& h, const float scr[4], const float& nr, const float& fr,
				const float& fov, const point3& pos, const point3& look, const vec3& up){
			screen[0] = scr[0];
			screen[1] = scr[1];
			screen[2] = scr[2];
			screen[3] = scr[3];

			near = nr;
			far = fr;

			float m[4][4] = {
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, fr / (fr - nr), -(fr*nr)/(fr - nr)},
				{0, 0, 0, 1}
			};

			float invTan = 1.f / tanf((fov * PI / 180.f) / 2.f);
			mat4 ms = mat4::scale(invTan, invTan, 1.f);
			mat4 mm = mat4(m[0]);

			cameraToScreen = mat4::scale(invTan, invTan, 1.f) * mat4(m[0]);
			worldToCamera = mat4::lookAt(pos, look, up);

			cameraToWorld = worldToCamera.inverse();
			worldToScreen = cameraToScreen.inverse() * worldToCamera;

			screenToRaster = mat4::scale(w, h, 1.f); 
			screenToRaster *= mat4::scale(1.f / (screen[1] - screen[0]),
						1.f / (screen[2] - screen[3]), 1.f);
			screenToRaster *= mat4::translate(-screen[0], -screen[3], 0.f);

			rasterToScreen = screenToRaster.inverse();
			rasterToCamera = cameraToScreen.inverse() * rasterToScreen;
		}

		void getRay(const int& x, const int& y, ray& r);

	private:
		float screen[4];
		float near, far;
		mat4 cameraToScreen, screenToRaster, rasterToScreen,
			 rasterToCamera, worldToCamera, worldToScreen, cameraToWorld;
};

#endif
