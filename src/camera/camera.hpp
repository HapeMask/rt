#ifndef __RT_CAMERA__
#define __RT_CAMERA__

#include "mathlib/vector.hpp"
#include "mathlib/matrix.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"
#include "mathlib/constants.hpp"
#include "mathlib/utility.hpp"

#include <cmath>

class camera {
	public:
		camera(const float& w, const float& h, const float& fov, const point3& pos,
				const point3& look, const vec3& up){

			dir_ = normalize(vec3(look) - vec3(pos));
			right_ = cross(dir_, normalize(up));
			up_ = cross(right_, dir_);

			pos_ = pos;

			fov_ = fov;
			w_ = w;
			h_ = h;

			halfW_ = w/2;
			halfH_ = h/2;

			aspect_ = w/h;

			tf_  = tan(radians(fov));
		}

		void getRay(const int& x, const int& y, ray& r);

	private:
		int w_, h_;
		int halfW_, halfH_;
		float fov_, aspect_, tf_;
		vec3 dir_, up_, right_;
		point3 pos_;
};

#endif
