#ifndef __RAY__
#define __RAY__

#include "point.hpp"
#include "vector.hpp"

class ray {
	public:
		ray(){}

		ray(const point3& o, const vec3& d){
			origin = point3(o);
			direction = vec3(d);
		}

		point3 origin;
		vec3 direction;
};

#endif
