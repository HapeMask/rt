#pragma once
#include <memory>

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/transformation.hpp"

using std::shared_ptr;

#undef near
#undef far
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

        inline const point3& getPosition() const {
            return pos;
        }

        inline const point3& getLook() const {
            return look;
        }

        inline const vec3& getUp() const {
            return up;
        }

        inline const float& getFov() const {
            return fov;
        }

        inline void setPosition(const point3& p) {
            pos = p;
            updateMatrices();
        }

        inline void setLook(const point3& l) {
            look = l;
            updateMatrices();
        }

        inline void setFov(const float& f) {
            fov = f;
            updateMatrices();
        }

	private:
        float screen[4];
        float _width, _height;
		float near, far, fov;
		transform3d cameraToScreen, screenToRaster, rasterToCamera,
					worldToCamera;

        void updateMatrices();
        point3 pos, look;
        vec3 up;
        //vec3 pos,look,up;
};

typedef shared_ptr<camera> cameraPtr;
