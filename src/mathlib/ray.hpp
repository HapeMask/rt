#ifndef __RAY__
#define __RAY__

#include "point.hpp"
#include "vector.hpp"

class ray {
	public:
		ray(){}

		ray(const point3& o, const vec3& d){
			origin_ = point3(o);
			direction_ = vec3(d);
		}

		point3 origin_;
		vec3 direction_;
};

#endif
