#include <iostream>

#include "point.hpp"
#include "vector.hpp"

using namespace std;

point2::point2(const vec2& u){
	coords[0] = u(0);
	coords[1] = u(1);
}

point3::point3(const vec3& u){
	coords[0] = u(0);
	coords[1] = u(1);
	coords[2] = u(2);
}

ostream& operator<<(ostream& out, const point3& p){
	out << "point3(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
	return out;
}
