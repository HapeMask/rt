#include <iostream>
#include <cmath>

#include "sse.hpp"
#include "vector.hpp"
#include "point.hpp"

using namespace std;

vec3::vec3(const point3& p){
	values[0] = p.x();
	values[1] = p.y();
	values[2] = p.z();
    values[3] = 0;
}

ostream& operator<<(ostream& out, const vec2& x){
	out << "vec2(" << x(0) << ", " << x(1) << ")";
	return out;
}

ostream& operator<<(ostream& out, const vec3& x){
	out << "vec3(" << x(0) << ", " << x(1) << ", " << x(2) << ")";
	return out;
}

ostream& operator<<(ostream& out, const vec4& x){
	out << "vec4(" << x(0) << ", " << x(1) << ", " << x(2) << ", " << x(3) <<  ")";
	return out;
}
