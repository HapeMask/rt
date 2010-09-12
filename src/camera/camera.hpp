#pragma once

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
#include "mathlib/transformation.hpp"

#include <tr1/memory>
using std::tr1::shared_ptr;

class camera {
	public:
		camera(const float& w, const float& h, const float& nr, const float& fr,
				const float& fo, const point3& p, const point3& l, const vec3& u);

		const ray getRay(const float& x, const float& y) const;

        inline const float& width() const{
            return _width;
        }

        inline const float& height() const{
            return _height;
        }

        void move(const vec3& dir);

        inline const vec3& getPosition() const {
            return pos;
        }

        inline const vec3& getLook() const {
            return look;
        }

        inline const vec3& getUp() const {
            return up;
        }

	private:
        float screen[4];
        float _width, _height;
		float near, far, fov;
		transform3d cameraToScreen, screenToRaster, rasterToCamera,
					worldToCamera;

        void updateMatrices();
        vec3 pos,look,up;
};

typedef shared_ptr<camera> cameraPtr;
