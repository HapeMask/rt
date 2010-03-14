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

		const point3& origin() const {
			return origin_;
		}

		point3& origin(){
			return origin_;
		}

		const vec3& direction() const {
			return direction_;
		}

		vec3& direction(){
			return direction_;
		}

	private:
		point3 origin_;
		vec3 direction_;
};

#endif
