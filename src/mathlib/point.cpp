#include <iostream>

#include "point.hpp"
#include "vector.hpp"

using namespace std;

point2::point2(const vec2& v) : x_(v.x()), y_(v.y()) {}
point3::point3(const vec3& v) : xyzw(v.xyzw){}

ostream& operator<<(ostream& out, const point3& p){
	out << "point3(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
	return out;
}
