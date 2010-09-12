#ifndef __QUATERNION__
#define __QUATERNION__
#include <iostream>

#include "vector.hpp"
using namespace std;

class quaternion{
	public:
		quaternion(const float& w_, const float& x_, const float& y_, const float& z_){
			w = w_;
			x = x_;
			y = y_;
			z = z_;
		}

		quaternion(const float& w_, const vec3& v){
			w = w_;
			x = v(0);
			y = v(1);
			z = v(2);
		}

		float w;
		float x;
		float y;
		float z;

		inline quaternion inverse(){
			const float d = w*w + x*x + y*y + z*z;
			return quaternion(w/d, -x/d, -y/d, -z/d);
		}
};

quaternion qmult(const quaternion& a, const quaternion& b){
	vec3 q(a.x, a.y, a.z);
	vec3 r(b.x, b.y, b.z);
	return quaternion(((a.w * b.w) - dot(q, r)),
			vec3(((r * a.w) + (q * b.w) + cross(q, r))));
}

#endif
