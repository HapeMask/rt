#ifndef __RT_CAMERA__
#define __RT_CAMERA__

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
#include "mathlib/transformation.hpp"

#include <tr1/memory>
using std::tr1::shared_ptr;

class camera {
	public:
		camera(const float& w, const float& h, const float& nr, const float& fr,
				const float& fov, const point3& pos, const point3& look, const vec3& up);

		const ray getRay(const float& x, const float& y);

        inline const float& width() const{
            return _width;
        }

        inline const float& height() const{
            return _height;
        }

	private:
        float _width, _height;
		float near, far;
		transform3d cameraToScreen, screenToRaster, rasterToCamera,
					worldToCamera;
};

typedef shared_ptr<camera> cameraPtr;
#endif
