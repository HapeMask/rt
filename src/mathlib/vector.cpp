#include <iostream>
#include <cmath>

#include "sse.hpp"
#include "vector.hpp"
#include "point.hpp"

using std::ostream;

//vec3::vec3(const point3& p) : xyzw(p.xyzw) {}

ostream& operator<<(ostream& out, const vec2& v){
    out << "vec2(" << v.x << ", " << v.y << ")";
    return out;
}

ostream& operator<<(ostream& out, const vec3& v){
    out << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}

ostream& operator<<(ostream& out, const vec4& v){
    out << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w <<  ")";
    return out;
}
