#include "vector.hpp"

ostream& operator<<(ostream& out, const vec2& v){
	out << "vec3(" << v.x << ", " << v.y << ")";
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

ostream& operator<<(ostream& out, const point2& v){
	out << "point3(" << v.x << ", " << v.y << ")";
	return out;
}

ostream& operator<<(ostream& out, const point3& v){
	out << "point3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return out;
}

ostream& operator<<(ostream& out, const __m128& m){
    union {
        __m128 vec;
        struct {
            float w;
            float z;
            float y;
            float x;
        };
    } v = {m};
	out << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w <<  ")";
	return out;
}
