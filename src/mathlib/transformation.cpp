#include "transformation.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "matrix.hpp"
#include "utility.hpp"

#include <cmath>

transform3d translate(const float& dx, const float& dy, const float& dz){
	mat4 m;
	m(0,3) = dx;
	m(1,3) = dy;
	m(2,3) = dz;

	mat4 i;
	i(0,3) = -dx;
	i(1,3) = -dy;
	i(2,3) = -dz;

	return transform3d(m, i);
}

transform3d scale(const float& sx, const float& sy, const float& sz){
	mat4 m;
	m(0,0) = sx;
	m(1,1) = sy;
	m(2,2) = sz;

	mat4 i;
	i(0,0) = 1.f/sx;
	i(1,1) = 1.f/sy;
	i(2,2) = 1.f/sz;

	return transform3d(m, i);
}

transform3d lookAt(const point3& pos, const point3& look, const vec3& up){
	const vec3 dir(normalize(look - pos));
	const vec3 right(cross(dir, normalize(up)));
	const vec3 newUp(cross(right, dir));

    const float v[4][4] = {
        {right.x, newUp.x, dir.x, pos.x},
        {right.y, newUp.y, dir.y, pos.y},
        {right.z, newUp.z, dir.z, pos.z},
        {0.f, 0.f, 0.f, 1.f}
    };

    return transform3d(mat4(v).inverse());
}

transform3d perspective(const float& fov, const float& near, const float& far){
	const float invTan = 1.f / tan(radians(fov) / 2.f);
	const float v[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, far / (far - near), -(far*near)/(far - near)},
		{0, 0, 1, 0}
	};

	return scale(invTan, invTan, 1.f) * transform3d(mat4(v));
}
