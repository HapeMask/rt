#include <cmath>

#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"
#include "utility.hpp"

void makeCoordinateSystem(const vec3& u, vec3& v, vec3& w){
    if(abs(u.x) > abs(u.z)){
        const float invLen = 1.f / sqrt(u.x*u.x + u.y*u.y);
        v.x = -u.y * invLen;
        v.y = u.x * invLen;
        v.z = 0.f;
        v = normalize(v);
    }else{
        const float invLen = 1.f / sqrt(u.z*u.z + u.y*u.y);
        v.x = 0.f;
        v.y = -u.z * invLen;
        v.z = u.y * invLen;
    }
    w = normalize(cross(u,v));
}
