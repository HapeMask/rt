#ifndef __RT_CAMERA__
#define __RT_CAMERA__

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "mathlib/point.hpp"

class camera {
	public:
		/**
		 * Construct a camera with
		 * 	film width/height: h/w
		 * 	up vector: u
		 * 	forward vector: fw
		 * 	film plane near-distance : ||fw||
		 * 	at point p.
		 */
		camera(const int& w, const int& h,
				const float& f,
				const vec3& u, const vec3& fw,
				const point3& p){
			width = w;
			height = h;
			halfW = w/2;
			halfH = h/2;
			fov = f/2.f;
			up = u;
			forward = fw;
			left = cross(up, forward);
			position = p;
			near = fw.length();
		}

		void getRay(const int& x, const int& y, ray& r);

	private:
		int width, halfW;
		int height, halfH;
		float fov;
		vec3 up;
		vec3 forward;
		vec3 left;
		point3 position;
		float near;
};

#endif

